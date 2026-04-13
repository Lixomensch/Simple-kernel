#include "../include/vfs.h"
#include <stddef.h>

fs_node_t *fs_root = NULL;

uint32_t vfs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    if (node->read != NULL) {
        return node->read(node, offset, size, buffer);
    }
    return 0;
}

uint32_t vfs_write(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
    if (node->write != NULL) {
        return node->write(node, offset, size, buffer);
    }
    return 0;
}

void vfs_open(fs_node_t *node) {
    if (node->open != NULL) {
        node->open(node);
    }
}

void vfs_close(fs_node_t *node) {
    if (node->close != NULL) {
        node->close(node);
    }
}

struct dirent * vfs_readdir(fs_node_t *node, uint32_t index) {
    if ((node->flags & 0x07) == FS_DIRECTORY && node->readdir != NULL) {
        return node->readdir(node, index);
    }
    return NULL;
}

fs_node_t * vfs_finddir(fs_node_t *node, char *name) {
    if ((node->flags & 0x07) == FS_DIRECTORY && node->finddir != NULL) {
        return node->finddir(node, name);
    }
    return NULL;
}
