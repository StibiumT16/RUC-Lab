/*
Filesystem Lab disigned and implemented by Liang Junkai,RUC
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fuse.h>
#include <errno.h>
#include <stdbool.h>
#include <setjmp.h>
#include "disk.h"

#define DIRMODE 0040000|0755
#define REGMODE 0100000|0644

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))

jmp_buf myerror;

typedef struct{
	time_t atime, ctime, mtime; //3 * 8 bytes 
	unsigned int size; // 4 bytes
	unsigned short indirect_ptr;// 2 bytes 一个block有2k个指针, 表示8MB大小的文件
	bool directory;// 1 bytes => 2 bytes
}inode;//32B，一共 2^15 个inode,一block中128个inode, inode block有256个
//blk 0: inode bitmap
//blk 1,2: datablock bitmap
//blk 3-258: inode block
//blk 259-65535:data block

typedef char block_item[BLOCK_SIZE];

typedef struct{//目录项 32bytes
	char filename[26];
	unsigned short inode_num;
	int padding;
}dir_item;

//bitmap
typedef unsigned long long bitmap_item; 
typedef bitmap_item inode_bitmap[512];
typedef bitmap_item datablock_bitmap[1024];

void D_disk_read(block_item ptr){//read datablock bitmap
	disk_read(1, ptr);
	disk_read(2, ptr + BLOCK_SIZE);
}
void D_disk_write(block_item ptr){//write datablock bitmap
	disk_write(1, ptr);
	disk_write(2, ptr + BLOCK_SIZE);
}

bool bitmap_get_bit(bitmap_item* bp, int pos){
	return (bp[pos / 64] >> (pos % 64)) & 1;
}

void bitmap_set_bit(bitmap_item* bp, int pos){
	bp[pos / 64] |= 1ULL << (pos % 64);
}

void bitmap_reset_bit(bitmap_item* bp, int pos){
	bp[pos / 64] &= ~(1ULL << (pos % 64));
}

int inode_bitmap_cnt(bitmap_item *bp){
	int res = 0;
	for(int i = 0; i < 32768; ++i)
		if(bitmap_get_bit(bp, i)) ++res;
	return res;
}

int datablock_bitmap_cnt(bitmap_item *bp){
	int res = 0;
	for(int i = 259; i <= 65535; ++i)
		if(bitmap_get_bit(bp, i)) ++res;
	return res;
}

//block
unsigned short new_block(){
	datablock_bitmap bp;
	D_disk_read((void *)bp);
	for(int i = 259; i <= 65535; ++i)
		if(!bitmap_get_bit(bp, i)){
			bitmap_set_bit(bp, i);
			D_disk_write((void *)bp);
			return i;
		}
	longjmp(myerror, -ENOSPC);
}

void alloc_blocks(int t, unsigned short res[t]){
	if(!t) return;
	int cnt = 0;
	datablock_bitmap bp;
	D_disk_read((void *)bp);
	for(int i = 259; i <= 65535; ++i)
		if(!bitmap_get_bit(bp, i)){
			res[cnt++] = i;
			if(cnt == t) break;
		}
	if(cnt < t) longjmp(myerror, -ENOSPC);
	for(int i = 0; i < t; ++i) bitmap_set_bit(bp, res[i]);
	D_disk_write((void *)bp);
}
 
void del_block(unsigned short block_num){
	datablock_bitmap bp;
	D_disk_read((void *)bp);
	if(!bitmap_get_bit(bp, block_num)) return;
	bitmap_reset_bit(bp, block_num);
	char empty[4096];
	memset(empty, 0, sizeof(empty));
	disk_write(block_num, empty);
	D_disk_write((void *)bp);
}

void del_blocks(unsigned short *block_num, int t){
	if(!t) return;
	datablock_bitmap bp;
	D_disk_read((void *)bp);
	char empty[4096];
	memset(empty, 0, sizeof(empty));
	for(int i = 0; i < t; ++i){
		if(!bitmap_get_bit(bp, block_num[i])) continue;
		bitmap_reset_bit(bp, block_num[i]);
		disk_write(block_num[i], empty);
		//block_num[i] = 0;
	}
	D_disk_write((void *)bp);
}

bool find_enough_block(int t){
	if(!t) return 1;
	datablock_bitmap bp;
	D_disk_read((void *)bp);
	for(int i = 259; i <= 65535; ++i)
		if(!bitmap_get_bit(bp, i) && !(--t)) return true;
	return false;
}

int get_num_of_block(size_t size){
	return size / BLOCK_SIZE + (size % BLOCK_SIZE > 0);
}


//inode
inode get_inode(unsigned short inode_num){	
	unsigned short blk_num = 3 + inode_num / 128;//块编号
	size_t offset = (inode_num % 128) * 32;//块中偏移量
	block_item blk;
	disk_read(blk_num, blk);
	return *(inode *)(blk + offset);
}

void set_inode(inode cur, unsigned short inode_num){
	unsigned short blk_num = 3 + inode_num / 128;
	size_t offset = (inode_num % 128) * 32;
	block_item blk;
	disk_read(blk_num, blk);
	*(inode *)(blk + offset) = cur;
	disk_write(blk_num, blk);
}

unsigned short new_inode(){
	inode_bitmap bp;
	disk_read(0, bp);
	for(int i = 1; i < 32768; ++i)
		if(!bitmap_get_bit(bp, i)){
			bitmap_set_bit(bp, i);
			disk_write(0, bp);
			return i;
		}
	longjmp(myerror, -ENOSPC);
}

void del_inode(unsigned short inode_num){
	inode_bitmap bp;
	disk_read(0, bp);
	bitmap_reset_bit(bp, inode_num);
	disk_write(0, bp);
}

bool find_enough_inode(int t){
	if(!t) return 1;
	inode_bitmap bp;
	disk_read(0, bp);
	for(int i = 1; i < 32768; ++i)
		if(!bitmap_get_bit(bp, i) && !(--t))
			return 1;
	return 0;
}

void init_inode(unsigned short inode_num, bool directory){
	inode tmp;
	tmp.atime = tmp.ctime = tmp.mtime = time(NULL);
	tmp.size = 0;
	tmp.indirect_ptr = new_block();
	tmp.directory = directory;
	set_inode(tmp, inode_num);
}

void destory_inode(unsigned short inode_num){
	inode tmp = get_inode(inode_num);
	block_item blk;
	disk_read(tmp.indirect_ptr, blk);
	del_blocks((unsigned short *)blk, get_num_of_block(tmp.size));
	del_block(tmp.indirect_ptr);
	del_inode(inode_num);
}

//directory
void get_dir(inode dir_inode, dir_item *res) {
    block_item dir_blk;
    disk_read(dir_inode.indirect_ptr, dir_blk);
    unsigned short *blk_num = (unsigned short *)dir_blk;
    char *ptr = (char *)res;
	int num_of_blk = get_num_of_block(dir_inode.size);
    for (int i = 0; i < num_of_blk; ++i, ptr += BLOCK_SIZE) 
        disk_read(blk_num[i], ptr);
}

void set_dir(inode dir_inode, dir_item *dir){
	block_item dir_blk; 
	disk_read(dir_inode.indirect_ptr, dir_blk);
	unsigned short *blk_num = (unsigned short *) dir_blk;
	char *ptr = (char *) dir; 
	int num_of_blk = get_num_of_block(dir_inode.size);
	for(int i = 0; i < num_of_blk; ++i, ptr += BLOCK_SIZE) disk_write(blk_num[i], ptr);
	int cnt = 0;
	for(int j = num_of_blk; j < 2048 &&  blk_num[j]; ++j) ++cnt;
	del_blocks(&blk_num[num_of_blk], cnt);
	disk_write(dir_inode.indirect_ptr, dir_blk);
}

void expand_dir(inode dir_inode){
	int num_of_block = get_num_of_block(dir_inode.size + 1);
	if(get_num_of_block(dir_inode.size) == num_of_block) return;
	block_item dir_blk;
	disk_read(dir_inode.indirect_ptr, dir_blk);
	unsigned short *blk_num = (unsigned short *) dir_blk;
	blk_num[num_of_block - 1] = new_block();
	disk_write(dir_inode.indirect_ptr, dir_blk);
}

int get_num_of_dir_item(inode dir_inode){
	return dir_inode.size / sizeof(dir_item);
}

dir_item* find_dir(dir_item* dir, int cnt, const char* filename, int len){
	char name[24];
	memset(name, 0, 24);
	memcpy(name, filename, len);
	for(dir_item *i = dir; i != dir + cnt; ++i)
		if(!memcmp(i->filename, name, 24)) return i;
	return NULL;
}

//path
inode path2inode(const char *path, unsigned short *inode_num){
	inode cur = get_inode(0);
	if(inode_num) *inode_num = 0;
	while(1){
		if(*path == '/') ++path;
		if(!*path) break;
		int len = strchrnul(path, '/') - path;
		if(!cur.directory) longjmp(myerror, -ENOTDIR);
		static dir_item *ptr, res[32768];
		get_dir(cur, res);
		ptr = find_dir(res, get_num_of_dir_item(cur), path, len);
		if(ptr){
			cur = get_inode(ptr->inode_num);
			if(inode_num) *inode_num = ptr->inode_num;
			path += len;
		}
		else longjmp(myerror, -ENOENT);
	}
	return cur;
}

/*
inode path2inode(const char *path, unsigned short *inode_num){
	inode cur = get_inode(0);
	if(inode_num) *inode_num = 0;
	int len = strlen(path), head = 0, tail = 0, k; 
	char mypath[len + 10], filename[25];
	strncpy(mypath, path, len + 10);
	if(len == 1 && path[0] == '/') return cur;
	if(path[len - 1] == '/') --len;
	mypath[len] = '\0';
	for(int i = head + 1; i < len; ++i){
		if(mypath[i] != '/') continue;
		tail = i, k = 0;
		for(int j = head + 1; j < tail; ++j, ++k) filename[k] = mypath[j];
		filename[k++] = '\0';
		static dir_item *ptr, res[32768];
		get_dir(cur, res);
		ptr = find_dir(res, get_num_of_dir_item(cur), filename, k);
		if(ptr){
			cur = get_inode(ptr->inode_num);
			if(inode_num) *inode_num = ptr->inode_num;
		}
		else longjmp(myerror, -ENOENT);
		head = tail , i = head;
	}
	k = 0;
	for(int j = head + 1; j < len; ++j, ++k) filename[k] = mypath[j];
	filename[k++] = '\0';
	static dir_item *ptr, res[32768];
	get_dir(cur, res);
	ptr = find_dir(res, get_num_of_dir_item(cur), filename, k);
	if(ptr){
		cur = get_inode(ptr->inode_num);
		if(inode_num) *inode_num = ptr->inode_num;
	}
	else longjmp(myerror, -ENOENT);
	return cur;
}
*/

inode path2pinode(const char* path, unsigned short *res_num, char name[25], bool *use){
	inode cur = get_inode(0), res;
	unsigned short inode_num = 0;
	if(use) *use = true;
	bool no_use = false;
	while(1){
		if(*path == '/') ++path;
		if(!*path) break;
		if(no_use) longjmp(myerror, -ENOENT);
		int len = strchrnul(path, '/') - path;
		if(!cur.directory) longjmp(myerror, -ENOTDIR);
		static dir_item *ptr, tmp[32768];
		res = cur;
		get_dir(cur, tmp);
		ptr = find_dir(tmp, get_num_of_dir_item(cur), path, len);
		if(res_num) *res_num = inode_num;
		if(ptr){
			cur = get_inode(ptr->inode_num);
			if(res_num)inode_num = ptr->inode_num; 
		}
		else no_use = true;
		memset(name, 0, 25);
		strncpy(name, path, len);
		name[len] = '\0';
		path += len;
	}
	return res;
}

//Format the virtual block device in the following function
int mkfs(){
	init_inode(0, 1);
	return 0;
}

//Filesystem operations that you need to implement
int fs_getattr (const char *path, struct stat *attr){
	int error = setjmp(myerror);
	if(error) return error;
	inode tmp  = path2inode(path, NULL);
	attr->st_mode = tmp.directory ? DIRMODE : REGMODE;
	attr->st_nlink = 1;
	attr->st_uid = getuid();
	attr->st_gid = getgid();
	attr->st_size = tmp.size;
	attr->st_atime = tmp.atime;
	attr->st_mtime = tmp.mtime;
	attr->st_ctime = tmp.ctime;
	return 0;
}

int fs_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	int error = setjmp(myerror);
	if(error) return error;
	unsigned short inode_num;
	inode tmp = path2inode(path, &inode_num); 
	if(!tmp.directory) return -ENOTDIR;
	static dir_item dir[32768];
	get_dir(tmp, dir);
	for(dir_item* it = dir; it != dir + get_num_of_dir_item(tmp); ++it){
		char name[25];
        memset(name, 0, sizeof(name));
		memcpy(name, it->filename, 24);
		filler(buffer, name, NULL, 0);
	}
	tmp.atime = time(NULL), set_inode(tmp, inode_num);
	return 0;
}

int fs_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
	int error = setjmp(myerror);
	if(error) return error;
	unsigned short inode_num; 
	inode tmp = path2inode(path, &inode_num); 
	if(tmp.directory) return -EISDIR;
	block_item ptr;
	disk_read(tmp.indirect_ptr, ptr);
	unsigned short *blk_num = (unsigned short*) ptr;
	for(int i = offset / 4096, blk_offset = (offset & 0xFFF), blk_size, remain_size = size; (blk_size = min(4096 - blk_offset, remain_size)) != 0; ++i, blk_offset = 0, remain_size -= blk_size){
		block_item blk;
		disk_read(blk_num[i], blk);
		memcpy(buffer, &blk[blk_offset], blk_size);
		buffer += blk_size;
	}
	tmp.atime = time(NULL), set_inode(tmp, inode_num);
	return size;
}

int mk(const char *path, bool directory){
	int error = setjmp(myerror);
	if(error) return error;
	if(!find_enough_inode(1) || !find_enough_block(2)) return -ENOSPC;
	char name[25];  bool use; unsigned short inode_num;
	inode fa = path2pinode(path, &inode_num, name, &use);
	int cnt = get_num_of_dir_item(fa);
	static dir_item ptr[32768]; 
	get_dir(fa, ptr);
	expand_dir(fa), fa.size += sizeof(dir_item);
	ptr[cnt].inode_num = new_inode();
	memcpy(ptr[cnt].filename, name, 24);
	init_inode(ptr[cnt].inode_num, directory);
	fa.atime = fa.mtime = fa.ctime = time(NULL); 
	set_dir(fa, ptr), set_inode(fa, inode_num);
	return 0;
}

int fs_mknod (const char *path, mode_t mode, dev_t dev){
	return mk(path, 0);
}

int fs_mkdir (const char *path, mode_t mode){
	return mk(path, 1);
}

int rm(const char *path){
	int error = setjmp(myerror);
	if(error) return error;
	char name[25]; unsigned short inode_num;
	inode fa = path2pinode(path, &inode_num, name, NULL);
	static dir_item dir[32768];
	get_dir(fa, dir);
	dir_item* ptr = find_dir(dir, get_num_of_dir_item(fa), name, strlen(name));
	if(!ptr) return -ENOENT;
	destory_inode(ptr->inode_num);
	memcpy(ptr, ptr + 1, sizeof(dir_item) * ((dir + get_num_of_dir_item(fa)) - (ptr + 1)));
	fa.ctime = fa.mtime = time(NULL), fa.size -= sizeof(dir_item);
	set_dir(fa, dir), set_inode(fa, inode_num);
	return 0;
}

int fs_rmdir (const char *path){
	return rm(path);
}

int fs_unlink (const char *path){
	return rm(path);
}

int fs_rename (const char *oldpath, const char *newpath){
	if(!strcmp(oldpath, newpath)) return 0;
	fs_unlink(newpath);
	int error = setjmp(myerror);
	if(error) return error;
	char from_name[25], to_name[25]; unsigned short from_num ,to_num;
	inode from = path2pinode(oldpath, &from_num, from_name, NULL), to = path2pinode(newpath, &to_num, to_name, NULL);
	static dir_item from_dir[32768];
    get_dir(from, from_dir);
	dir_item *from_ptr = find_dir(from_dir, get_num_of_dir_item(from), from_name, strlen(from_name));
	from.atime = from.mtime = time(NULL);
	if(!from_ptr) return -ENOENT;
	if(from_num == to_num) memcpy(from_ptr->filename, to_name, 24);
	else{
        static dir_item to_dir[32768];
        get_dir(to, to_dir);
		dir_item *to_ptr = find_dir(to_dir, get_num_of_dir_item(to), to_name, strlen(to_name));
		if(to_ptr) return -EEXIST;
		expand_dir(to);
		int cnt = get_num_of_dir_item(to);
		memcpy(to_dir[cnt].filename, to_name, 24);
		to_dir[cnt].inode_num = from_ptr->inode_num;
		to.size += sizeof(dir_item), to.atime = to.mtime = time(NULL);
		set_dir(to, to_dir), set_inode(to, to_num);
		memcpy(from_ptr, from_ptr + 1, sizeof(dir_item) * ((from_dir + get_num_of_dir_item(from)) - (from_ptr + 1)));
		from.size -= sizeof(dir_item);
	}
	set_dir(from, from_dir), set_inode(from, from_num);
	return 0;
}

int fs_write (const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
	int error = setjmp(myerror);
	if(error) return 0;
	unsigned short inode_num; 
	inode tmp = path2inode(path, &inode_num); 
	if(tmp.directory) return -EISDIR;
	block_item ptr;
	disk_read(tmp.indirect_ptr, ptr);
	unsigned short * blk_num = (unsigned short*) ptr;
	int cnt = 0;
	for(int i = offset / 4096, blk_offset = (offset & 0xFFF), blk_size, remain_size = size; (blk_size = min(4096 - blk_offset, remain_size)) != 0; ++i, blk_offset = 0, remain_size -= blk_size)
		if(!blk_num[i]) ++cnt;
	unsigned short new_blk[cnt];
	alloc_blocks(cnt, new_blk);
	for(int i = offset / 4096, blk_offset = (offset & 0xFFF), blk_size, remain_size = size; (blk_size = min(4096 - blk_offset, remain_size)) != 0; ++i, blk_offset = 0, remain_size -= blk_size){
		if(!blk_num[i]) blk_num[i] = new_blk[--cnt];
		block_item blk;
		disk_read(blk_num[i], blk);
		memcpy(&blk[blk_offset], buffer, blk_size);
		buffer += blk_size;
		disk_write(blk_num[i], blk);
	}
	disk_write(tmp.indirect_ptr, ptr);
	tmp.mtime = tmp.ctime = time(NULL), tmp.size = max(tmp.size, size + offset);
	set_inode(tmp, inode_num);
	return size;
}

int fs_truncate (const char *path, off_t size){
	int error = setjmp(myerror);
	if(error) return error;
	unsigned short inode_num;
	inode tmp = path2inode(path, &inode_num); 
	if(tmp.directory) return -EISDIR;
	block_item ptr;
	disk_read(tmp.indirect_ptr, ptr);
	int num1 =  get_num_of_block(tmp.size), num2 = get_num_of_block(size);
	unsigned short * blk_num = (unsigned short*) ptr;
	if(size > tmp.size){
		int need =  num2 - num1; unsigned short new_blk[need];
		alloc_blocks(need, new_blk);
		for(int i = num1; i < num2; ++i) blk_num[i] = new_blk[--need];
	}
	else{
		int need = size % BLOCK_SIZE;
		if(need) {
			block_item last;
			disk_read(blk_num[need - 1], last);
			memset(last + need, 0, BLOCK_SIZE - need);
			disk_write(blk_num[need - 1], last);
		}
		del_blocks(&blk_num[num2], num1 - num2);
	}
	tmp.size = size, tmp.ctime = time(NULL);
	disk_write(tmp.indirect_ptr, ptr);
	set_inode(tmp, inode_num);
	return 0;
}

int fs_utime (const char *path, struct utimbuf *buffer){
	int error = setjmp(myerror);
	if(error) return error;
	unsigned short inode_num;
	inode tmp  = path2inode(path, &inode_num); 
	tmp.atime = buffer->actime;
	tmp.mtime = buffer->modtime;
	tmp.ctime = time(NULL);
	set_inode(tmp, inode_num);
	return 0;
}

int fs_statfs (const char *path, struct statvfs *stat){
	inode_bitmap i;
	datablock_bitmap d;
	disk_read(0, i);
	D_disk_read((void *)d);
	stat->f_bsize = BLOCK_SIZE;
	stat->f_blocks = 65277;
	stat->f_bavail = stat->f_bfree = 65277 - datablock_bitmap_cnt(d);
	stat->f_files = 32768;
	stat->f_ffree = stat->f_favail = 32767 - inode_bitmap_cnt(i);
	stat->f_namemax = 24;
	return 0;
}

int fs_open (const char *path, struct fuse_file_info *fi){
	printf("Open is called:%s\n",path);
	return 0;
}

//Functions you don't actually need to modify
int fs_release (const char *path, struct fuse_file_info *fi){
	printf("Release is called:%s\n",path);
	return 0;
}

int fs_opendir (const char *path, struct fuse_file_info *fi){
	printf("Opendir is called:%s\n",path);
	return 0;
}

int fs_releasedir (const char * path, struct fuse_file_info *fi){
	printf("Releasedir is called:%s\n",path);
	return 0;
}

static struct fuse_operations fs_operations = {
	.getattr    = fs_getattr,
	.readdir    = fs_readdir,
	.read       = fs_read,
	.mkdir      = fs_mkdir,
	.rmdir      = fs_rmdir,
	.unlink     = fs_unlink,
	.rename     = fs_rename,
	.truncate   = fs_truncate,
	.utime      = fs_utime,
	.mknod      = fs_mknod,
	.write      = fs_write,
	.statfs     = fs_statfs,
	.open       = fs_open,
	.release    = fs_release,
	.opendir    = fs_opendir,
	.releasedir = fs_releasedir
};

int main(int argc, char *argv[]){
	if(disk_init()){
		printf("Can't open virtual disk!\n");
		return -1;
		}
	if(mkfs()){
		printf("Mkfs failed!\n");
		return -2;
		}
    return fuse_main(argc, argv, &fs_operations, NULL);
}

