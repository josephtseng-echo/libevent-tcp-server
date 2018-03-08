/*
 * inter_task.c
 * Copyright (C) 2018 josephzeng <josephzeng36@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "core/z_type.h"
#include "core/z_event.h"
#include "core/z_task.h"
#include "core/z_utils.h"

void onInterTaskRead(struct bufferevent *bev, void *arg){

	struct zsq_task *task = (struct zsq_task *)arg;
	check_condition_void(task && (task->m_bev == bev));
	struct evbuffer *input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	if(len > ZSQ_INTERPACKET_HEADER_LENGTH){
		//check packet header decode
		struct ZsqInternalHeader h;
		evbuffer_copyout(input, &h, sizeof(h));
		check_condition_void(z_inter_packet_decode(h));
		//get packet buff connect
		char packet_buff[ZSQ_INTERPACKET_MAX_LENGTH];
		uint32 dlen = len > sizeof(packet_buff) ? sizeof(packet_buff) : len;
		struct ZsqInternalHeader *head = (struct ZsqInternalHeader*)packet_buff;
		evbuffer_copyout(input, head, dlen);
		evbuffer_drain(input, dlen);

		unsigned char data[head->bodylen - 4];
		z_get_packet_body((unsigned char*)&data, head);
		printf("body1 = %s \n", data);

	}else{
		evbuffer_drain(input, len);
	}
	sendData(bev, "12345", 5);
	/*
	struct zsq_task *abc = z_get_task_by_id(task->m_id);
	*/
}

void onInterTaskWrite(struct bufferevent *bev, void *arg){
	struct zsq_task *task = (struct zsq_task *)arg;
	check_condition_void(task && (task->m_bev == bev));
}

void onInterTaskError(struct bufferevent *bev, short events, void *arg){
	struct zsq_task *task = (struct zsq_task *)arg;
	check_condition_void(task && (task->m_bev == bev));
	if (events & BEV_EVENT_EOF) {
		//client close
	} else if (events & BEV_EVENT_ERROR) {
		//server error
	} else if (events & BEV_EVENT_TIMEOUT) {
		//timeout
	}
	bufferevent_free(bev);
	z_remove_task(task);
	free(task);
}
