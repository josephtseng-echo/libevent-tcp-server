/*
 * main.c
 * Copyright (C) 2018 josephzeng <josephzeng36@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */


#include "core/z_type.h"
#include "core/z_event.h"
#include "core/z_task.h"
#include "task.c"
#include "inter_task.c"

void onTimer(int32 fd, int16 which, void *arg){
	struct zsq_event_timer *zet = arg;
	uint32 id = zet->id;
	printf("ok...%i\n", zet->id);
	evtimer_add(zsq_m_event_map[id], &zsq_m_timeval_map[id]);
}

void onInterTaskAccept(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *arg){
	struct zsq_task *task;
	task = malloc(sizeof(struct zsq_task));
	struct event_base *base = (struct event_base *)arg;
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE );
	if (!bev) {
		fprintf(stderr, "error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	int ret = bufferevent_enable(bev, BEV_OPT_THREADSAFE );
	if(ret < 0){
		fprintf(stderr, "bufferevent_enable fail.");
		return;
	}
	//
	uint32 m_fd = (uint32)fd;
	task->m_bev = bev;
	task->m_fd = m_fd;
	task->m_id = 0;
	//add task
	printf("init inter task\n");
	int32 at_ret = z_add_task(task);
	bufferevent_setcb(bev, onInterTaskRead, onInterTaskWrite, onInterTaskError, (void *)task);
	bufferevent_enable(bev, EV_READ);
	bufferevent_setwatermark(bev, EV_READ, 0, 0);

	//add timer event
	//zsq_server_add_event(base, m_fd, 1, onTimer, (void*)base);
	//zsq_server_add_event(base, 1000, 1, onTimer, (void*)base);
}

void onTaskAccept(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *arg){
	struct zsq_task *task;
	task = malloc(sizeof(struct zsq_task));
	struct event_base *base = (struct event_base *)arg;
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE );
	if (!bev) {
		fprintf(stderr, "error constructing bufferevent!");
		event_base_loopbreak(base);
		return;
	}
	int ret = bufferevent_enable(bev, BEV_OPT_THREADSAFE );
	if(ret < 0){
		fprintf(stderr, "bufferevent_enable fail.");
		return;
	}
	//
	uint32 m_fd = (uint32)fd;
	task->m_bev = bev;
	task->m_fd = m_fd;
	task->m_id = 0;
	printf("init  task\n");
	//add task
	int32 at_ret = z_add_task(task);
	bufferevent_setcb(bev, onTaskRead, onTaskWrite, onTaskError, (void *)task);
	bufferevent_enable(bev, EV_READ);
	bufferevent_setwatermark(bev, EV_READ, 0, 0);
}

bool server_init(){
	evthread_use_pthreads();
	zsq_main_event = malloc(sizeof(struct zsq_event));
	zsq_main_event->m_base = event_base_new();

	check_condition(zsq_server_task_init(zsq_main_event, "127.0.0.1", 9999, onTaskAccept), false);
	check_condition(zsq_server_task_init(zsq_main_event, "127.0.0.1", 9998, onInterTaskAccept), false);

	//add timer event
	//zsq_server_add_event(zsq_main_event->m_base, 20000, 1, onTimer, (void*)zsq_main_event->m_base);

	event_base_dispatch(zsq_main_event->m_base);
	event_base_free(zsq_main_event->m_base);
	free(zsq_main_event);
	return true;
}

int main(int argc, char **argv){
	server_init();
	return EXIT_SUCCESS;
}
