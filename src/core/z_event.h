#pragma once
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"
#include "event2/event_compat.h"
#include <stdbool.h>

struct zsq_event {
	struct event_base *m_base;
};

struct zsq_event_timer {
	uint32 id;
};

static struct zsq_event *zsq_main_event;
struct timeval zsq_m_timeval_map[65355];
struct event *zsq_m_event_map[65355];

static void zsq_server_add_event(struct event_base *base, const uint32 id, const uint32 t, event_callback_fn f, void *arg){
	evutil_timerclear(&zsq_m_timeval_map[id]);
	zsq_m_timeval_map[id].tv_usec = 0;
	zsq_m_timeval_map[id].tv_sec = t;
	struct zsq_event_timer *zet;
	zet = malloc(sizeof(struct zsq_event_timer));
	zet->id = id;
	zsq_m_event_map[id] = evtimer_new(base, f, (void*)zet);
	evtimer_add(zsq_m_event_map[id], &zsq_m_timeval_map[id]);
}

static void zsq_server_remove_event(const uint32 id){
	if(zsq_m_event_map[id] != NULL){
		evtimer_del(zsq_m_event_map[id]);
	}
}

static bool zsq_server_task_init(struct zsq_event *ev, char* ip, uint16 port, evconnlistener_cb cb){
	struct evconnlistener *listener;
	struct event *signal_event;
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_port = htons(port);
	listener = evconnlistener_new_bind(ev->m_base, cb, (void *)ev->m_base,
		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE|LEV_OPT_THREADSAFE, -1,
		(struct sockaddr*)&sin,
		sizeof(sin));
	if (!listener) {
		fprintf(stderr, "could not create a listener!\n");
		return false;
	}
	return true;
}


