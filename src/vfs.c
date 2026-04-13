#include "../include/vfs.h"
#include <stddef.h>

fs_node_t *fs_root = NULL;

fs_node_t* vfs_create(fs_node_t* dir, const char *name) {
    if (dir && dir->create) {
        return dir->create(dir, name);
    }
    return NULL;
}

uint32_t vfs_write(fs_node_t *node, const char *data, uint32_t size) {
    if (node && node->write) {
        return node->write(node, data, size);
    }
    return 0;
}

uint32_t vfs_read(fs_node_t *node, char *buffer, uint32_t size) {
    if (node && node->read) {
        return node->read(node, buffer, size);
    }
    return 0;
}

fs_node_t* vfs_find(fs_node_t *dir, const char *name) {
    if (dir && dir->find) {
        return dir->find(dir, name);
    }
    return NULL;
}

void vfs_list(fs_node_t *dir) {
    if (dir && dir->list) {
        dir->list(dir);
    }
}
