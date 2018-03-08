#pragma once
#include "z_type.h"

struct zsq_task {
	struct bufferevent *m_bev;
	uint32 m_fd;
	uint64 m_id;
};

struct zsq_task_data {
	struct zsq_task *task;
	uint64 id;
};

static struct zsq_task_data *zsq_task_array[65355];

static int32 z_add_task(struct zsq_task *task){
	struct zsq_task_data *item = (struct zsq_task_data*) malloc(sizeof(struct zsq_task_data));
	uint64 id = task->m_id;
	while(zsq_task_array[id] != NULL){
		if(zsq_task_array[id]->id == id){
			return 0;
		}
	}
	item->id = id;
	item->task = task;
	zsq_task_array[id] = item;
	return 1;
}

static int32 z_remove_task(struct zsq_task *task){
	uint64 id = task->m_id;
	while(zsq_task_array[id] != NULL){
		if(zsq_task_array[id]->id == id){
			zsq_task_array[id] = NULL;
			return 1;
		}
	}
	return 0;
}

static struct zsq_task* z_get_task_by_id(uint64 id){
	while(zsq_task_array[id] != NULL){
		if(zsq_task_array[id]->id == id){
			return (struct zsq_task*)zsq_task_array[id]->task;
		}
	}
	struct zsq_task *task = (struct zsq_task*) malloc(sizeof(struct zsq_task));
	return task;
}
