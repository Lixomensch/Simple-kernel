#ifndef FS_VFS_H
#define FS_VFS_H

#include <stdint.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02

struct fs_node;

typedef struct fs_node* (*create_type_t)(struct fs_node*, const char* name);
typedef uint32_t (*write_type_t)(struct fs_node*, const char* data, uint32_t size);
typedef uint32_t (*read_type_t)(struct fs_node*, char* buffer, uint32_t size);
typedef struct fs_node* (*find_type_t)(struct fs_node*, const char* name);
typedef void (*list_type_t)(struct fs_node*);

typedef struct fs_node {
    char name[64];
    uint32_t length;
    uint32_t flags;

    void* data;

    struct fs_node* parent;
    struct fs_node* next;
    struct fs_node* children;

    create_type_t create;
    write_type_t write;
    read_type_t read;
    find_type_t find;
    list_type_t list;
} fs_node_t;

extern fs_node_t *fs_root;

fs_node_t* vfs_create(fs_node_t* dir, const char *name);
uint32_t vfs_write(fs_node_t *node, const char *data, uint32_t size);
uint32_t vfs_read(fs_node_t *node, char *buffer, uint32_t size);
fs_node_t* vfs_find(fs_node_t *dir, const char *name);
void vfs_list(fs_node_t *dir);

#endif
