#include "../include/ramfs.h"
#include "../include/vfs.h"
#include "../include/kheap.h"
#include "../include/string.h"
#include "../include/kernel.h"

static fs_node_t *ramfs_root_node;

static uint32_t ramfs_write(fs_node_t *node, const char *data, uint32_t size) {
    if ((node->flags & 0x07) != FS_FILE) return 0;
    if (!node->data) return 0;

    char *buf = (char*)node->data;
    
    uint32_t offset = node->length;
    uint32_t to_write = size;
    
    if (offset + to_write > 4095) {
        to_write = 4095 - offset;
    }
    
    for(uint32_t i=0; i<to_write; i++) {
        buf[offset + i] = data[i];
    }
    node->length += to_write;
    
    return to_write;
}

static uint32_t ramfs_read(fs_node_t *node, char *buffer, uint32_t size) {
    if ((node->flags & 0x07) != FS_FILE) return 0;
    if (!node->data) return 0;

    char *buf = (char*)node->data;
    
    uint32_t to_read = size;
    if (to_read > node->length) {
        to_read = node->length;
    }
    
    for(uint32_t i=0; i<to_read; i++) {
        buffer[i] = buf[i];
    }
    
    return to_read;
}

static fs_node_t* ramfs_find(fs_node_t *dir, const char *name) {
    fs_node_t *child = dir->children;
    while (child) {
        if (strcmp(child->name, name) == 0) {
            return child;
        }
        child = child->next;
    }
    return NULL;
}

static void ramfs_list(fs_node_t *dir) {
    fs_node_t *child = dir->children;
    while (child) {
        kprint(" ");
        kprint(child->name);
        kprint(" (");
        char len_buf[16];
        itoa(child->length, len_buf);
        kprint(len_buf);
        kprint(" bytes)\n");
        child = child->next;
    }
}

static fs_node_t* ramfs_create(fs_node_t* dir, const char* name) {
    if ((dir->flags & 0x07) != FS_DIRECTORY) return NULL;
    
    if (ramfs_find(dir, name)) {
        kprint("Erro: Arquivo ja existe.\n");
        return NULL;
    }

    fs_node_t *new_node = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    if (!new_node) return NULL;

    int i = 0;
    while(name[i] && i < 63) {
        new_node->name[i] = name[i];
        i++;
    }
    new_node->name[i] = '\0';

    new_node->length = 0;
    new_node->flags = FS_FILE;
    new_node->data = kmalloc(4096); 
    
    char *buf = (char*)new_node->data;
    for(int j=0; j<4096; j++) buf[j] = 0;

    new_node->parent = dir;
    new_node->children = NULL;
    new_node->next = NULL;

    new_node->create = 0;
    new_node->write = &ramfs_write;
    new_node->read = &ramfs_read;
    new_node->find = 0;
    new_node->list = 0;

    if (dir->children == NULL) {
        dir->children = new_node;
    } else {
        fs_node_t *last = dir->children;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
    }

    return new_node;
}

void ramfs_init() {
    ramfs_root_node = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    if (!ramfs_root_node) kprint("PANIC RAMFS ALLOC\n");
    
    char rootname[] = "ramroot";
    for(int i=0; rootname[i]; i++) ramfs_root_node->name[i] = rootname[i];
    ramfs_root_node->name[sizeof(rootname)] = '\0';
    
    ramfs_root_node->length = 0;
    ramfs_root_node->flags = FS_DIRECTORY;
    ramfs_root_node->data = NULL;
    ramfs_root_node->parent = NULL;
    ramfs_root_node->next = NULL;
    ramfs_root_node->children = NULL;

    ramfs_root_node->create = &ramfs_create;
    ramfs_root_node->write = 0;
    ramfs_root_node->read = 0;
    ramfs_root_node->find = &ramfs_find;
    ramfs_root_node->list = &ramfs_list;

    fs_root = ramfs_root_node;
}
