#include <cstdint>
#include <elf.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/procfs.h>
#include <sys/user.h>
#include <unistd.h>

#include <optional>
#include <string>
#include <vector>

#include "eflags.h"

std::string ShowNT(uint32_t type) {
  if (type == NT_PRSTATUS) {
    return "NT_PRSTATUS";
  } else if (type == NT_PRFPREG) {
    return "NT_PRFPREG";
  } else if (type == NT_PRPSINFO) {
    return "NT_PRPSINFO";
  } else if (type == NT_TASKSTRUCT) {
    return "NT_TASKSTRUCT";
  } else if (type == NT_AUXV) {
    return "NT_AUXV";
  } else if (type == NT_FILE) {
    return "NT_FILE";
  } else if (type == NT_SIGINFO) {
    return "NT_SIGINFO";
  } else if (type == NT_X86_XSTATE) {
    return "NT_X86_XSTATE";
  } else {
    return "Unknown";
  }
}

std::string ConcatVectorOfString(const std::vector<std::string> &vec,
                                 const std::string &sep = "") {
  std::string ret = "{";
  for (const auto &s : vec) {
    if (ret.size() > 1) {
      ret += sep;
    }
    ret += s;
  }
  ret += "}";
  return ret;
}

std::vector<uint8_t> GetMemorySlice(uintptr_t addr, size_t size) {
  std::vector<uint8_t> ret;
  return ret;
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <path to core file>\n", argv[0]);
  }

  int fd = open(argv[1], O_RDONLY);
  size_t size = lseek(fd, 0, SEEK_END);
  size_t mapped_size = (size + 0xfff) & ~0xfff;
  const char *head = reinterpret_cast<const char *>(
      mmap(NULL, mapped_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));

  const Elf64_Ehdr *ehdr = reinterpret_cast<const Elf64_Ehdr *>(head);

  std::optional<unsigned long long int> rax = std::nullopt;
  for (size_t pi = 0; pi < ehdr->e_phnum; pi++) {
    const Elf64_Phdr *phdr =
        (reinterpret_cast<const Elf64_Phdr *>(head + ehdr->e_phoff)) + pi;
    if (phdr->p_type == PT_NOTE) {
      size_t offset_in_note = 0;
      while (offset_in_note < phdr->p_filesz) {
        const char *note = head + phdr->p_offset + offset_in_note;
        int32_t name_size =
            (*reinterpret_cast<const int32_t *>(note) + 3) / 4 * 4;
        int32_t desc_size =
            (*(reinterpret_cast<const int32_t *>(note) + 1) + 3) / 4 * 4;
        uint32_t type = *(reinterpret_cast<const uint32_t *>(note) + 2);
        const char *name = note + 4 * 3;
        if (type == NT_PRSTATUS) {
          // Show process info
          const prstatus_t *prstatus =
              reinterpret_cast<const prstatus_t *>(note + 4 * 3 + name_size);
          printf("PID: %d\n", prstatus->pr_pid);
          printf("PPID: %d\n", prstatus->pr_ppid);
          printf("Current signal: %d(%s)\n", prstatus->pr_cursig,
                 strsignal(prstatus->pr_cursig));

          // Show register info
          printf("Registers:\n");
          const struct user_regs_struct *regs =
              reinterpret_cast<const struct user_regs_struct *>(
                  prstatus->pr_reg);
          rax = regs->rax;
          printf("    r15:      0x%016llx\n", regs->r15);
          printf("    r14:      0x%016llx\n", regs->r14);
          printf("    r13:      0x%016llx\n", regs->r13);
          printf("    r12:      0x%016llx\n", regs->r12);
          printf("    rbp:      0x%016llx\n", regs->rbp);
          printf("    rbx:      0x%016llx\n", regs->rbx);
          printf("    r11:      0x%016llx\n", regs->r11);
          printf("    r10:      0x%016llx\n", regs->r10);
          printf("    r9:       0x%016llx\n", regs->r9);
          printf("    r8:       0x%016llx\n", regs->r8);
          printf("    rax:      0x%016llx\n", regs->rax);
          printf("    rcx:      0x%016llx\n", regs->rcx);
          printf("    rdx:      0x%016llx\n", regs->rdx);
          printf("    rsi:      0x%016llx\n", regs->rsi);
          printf("    rdi:      0x%016llx\n", regs->rdi);
          printf("    orig_rax: 0x%016llx\n", regs->orig_rax);
          printf("    rip:      0x%016llx\n", regs->rip);
          printf("    cs:       0x%016llx\n", regs->cs);
          printf("    eflags:   0x%016llx\n", regs->eflags);
          printf("              %s\n",
                 ConcatVectorOfString(X86EFlagsToStrings(regs->eflags), "|")
                     .c_str());
          printf("    rsp:      0x%016llx\n", regs->rsp);
          printf("    ss:       0x%016llx\n", regs->ss);
          printf("    fs_base:  0x%016llx\n", regs->fs_base);
          printf("    gs_base:  0x%016llx\n", regs->gs_base);
          printf("    ds:       0x%016llx\n", regs->ds);
          printf("    es:       0x%016llx\n", regs->es);
          printf("    fs:       0x%016llx\n", regs->fs);
          printf("    gs:       0x%016llx\n", regs->gs);
        } else if (type == NT_FILE) {
          printf("Files:\n");
          const char *file = note + 4 * 3 + name_size;
          uint64_t count = *reinterpret_cast<const uint64_t *>(file);
          uint64_t page_size =
              *reinterpret_cast<const uint64_t *>(file + sizeof(uint64_t));
          const char *filenames =
              file + sizeof(uint64_t) * 2 + count * 3 * sizeof(uint64_t);
          printf("    count: %lu pagesize: %lu\n", count, page_size);
          for (size_t i = 0; i < count; i++) {
            uint64_t start, end, file_ofs;
            start = *reinterpret_cast<const uint64_t *>(
                file + sizeof(uint64_t) * 2 + i * 3 * sizeof(uint64_t));
            end = *reinterpret_cast<const uint64_t *>(
                file + sizeof(uint64_t) * 2 + i * 3 * sizeof(uint64_t) +
                sizeof(uint64_t));
            file_ofs = *reinterpret_cast<const uint64_t *>(
                file + sizeof(uint64_t) * 2 + i * 3 * sizeof(uint64_t) +
                sizeof(uint64_t) * 2);
            printf("    start: 0x%016llx end: 0x%016llx page offset: 0x%016llx\n",
                   start, end, file_ofs);
            printf("    %s\n", filenames);
            filenames += 1 + strlen((char *)filenames);
          }
        }

        offset_in_note += 4 * 3 + name_size + desc_size;
      }
    }
  }
  return 0;
}
