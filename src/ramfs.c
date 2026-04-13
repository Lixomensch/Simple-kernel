#include "../include/ramfs.h"
#include "../include/kernel.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/vfs.h"

static fs_node_t *ramfs_root_node;
static fs_node_t *hello_node;

static char hello_content[] =
    "Ola Mundo! Este e o primeiro arquivo aberto pelo VFS.\nEle vive apenas na "
    "memoria (RAMFS)!\n";
static struct dirent dirent_hello;

static uint32_t ramfs_read(fs_node_t *node, uint32_t offset, uint32_t size,
                           uint8_t *buffer) {
  if (node == hello_node) {
    if (offset > node->length)
      return 0;
    if (offset + size > node->length) {
      size = node->length - offset;
    }

    char *ptr = hello_content + offset;
    for (uint32_t i = 0; i < size; i++) {
      buffer[i] = ptr[i];
    }
    return size;
  }
  return 0; 
}

static struct dirent *ramfs_readdir(fs_node_t *node, uint32_t index) {
  if (node == ramfs_root_node) {
    if (index == 0) {
      
      int i = 0;
      while (hello_node->name[i]) {
        dirent_hello.name[i] = hello_node->name[i];
        i++;
      }
      dirent_hello.name[i] = '\0';
      dirent_hello.ino = 1;
      return &dirent_hello;
    }
    
    return NULL;
  }
  return NULL;
}

static fs_node_t *ramfs_finddir(fs_node_t *node, char *name) {
  if (node == ramfs_root_node) {
    if (strcmp(name, "hello.txt") == 0) {
      return hello_node;
    }
  }
  return NULL;
}

void ramfs_init() {
  
  ramfs_root_node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
  if (!ramfs_root_node)
    kprint("PANIC RAMFS ALLOC\n");

  char rootname[] = "ramroot";
  for (int i = 0; rootname[i]; i++)
    ramfs_root_node->name[i] = rootname[i];
  ramfs_root_node->name[sizeof(rootname)] = '\0';

  ramfs_root_node->flags = FS_DIRECTORY;
  ramfs_root_node->read = 0;
  ramfs_root_node->write = 0;
  ramfs_root_node->open = 0;
  ramfs_root_node->close = 0;
  ramfs_root_node->readdir = &ramfs_readdir;
  ramfs_root_node->finddir = &ramfs_finddir;
  ramfs_root_node->ptr = 0;
  ramfs_root_node->impl = 0;

  
  hello_node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
  char filename[] = "hello.txt";
  for (int i = 0; filename[i]; i++)
    hello_node->name[i] = filename[i];
  hello_node->name[sizeof(filename)] = '\0';

  hello_node->flags = FS_FILE;

  
  uint32_t len = 0;
  while (hello_content[len])
    len++;
  hello_node->length = len;

  hello_node->read = &ramfs_read;
  hello_node->write = 0;
  hello_node->open = 0;
  hello_node->close = 0;
  hello_node->readdir = 0;
  hello_node->finddir = 0;
  hello_node->ptr = 0;
  hello_node->impl = 0;

  
  fs_root = ramfs_root_node;
}
