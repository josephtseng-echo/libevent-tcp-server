#pragma once
#include "z_utils.h"
#pragma pack(1)

struct ZsqExternalHeader {
	int8_t magic[2];
	uint8_t headlen;
	uint16_t bodylen;
	uint32_t maincmd;
	uint32_t subcmd;
	uint16_t code;
	uint8_t version;
	uint32_t reserved;
	uint8_t deal;
	char data[0];
};

struct ZsqInternalHeader {
	uint32_t maincmd;
	uint32_t subcmd;
	uint64_t userlinkid;
	uint32_t servlinkid;
	uint8_t protocols;
	uint32_t reserved;
	uint16_t bodylen;
	char data[0];
};

#pragma pack()
static const uint32_t ZSQ_PACKET_HEADER_LENGTH		= 21;
static const uint32_t ZSQ_INTERPACKET_HEADER_LENGTH = 27;
static const uint32_t ZSQ_PACKET_MAX_LENGTH         = 64*1024 - 22;
static const uint32_t ZSQ_INTERPACKET_MAX_LENGTH         = 64*1024 - 28;
static const unsigned int ZSQ_PACKET_MASK				= 0xFFFF;
static const unsigned int ZSQ_INTERNALPACKET_MASK		= 0xFFFFFFFF;

static void z_get_packet_body(unsigned char *body, struct ZsqInternalHeader *head){
	uint32_t body_len;
	memcpy(&body_len, head->data+0, 4);
	memcpy(body, head->data+4, body_len);
}

static bool z_inter_packet_decode(struct ZsqInternalHeader h){
	char *buff = (char*)&h;

	size_t offset = 0;
	size_t dlen = sizeof(h);
	h.maincmd = ntohs(*(uint32_t*)(buff + offset));
	offset += sizeof(uint32_t);
	check_condition(offset < dlen, false);

	h.subcmd = ntohs(*(uint32_t*)(buff + offset));
	offset += sizeof(uint32_t);
	check_condition(offset < dlen, false);

	h.userlinkid = *(uint64_t*)(buff + offset);
	offset += sizeof(uint64_t);
	check_condition(offset < dlen, false);

	h.servlinkid = *(uint32_t*)(buff + offset);
	offset += sizeof(uint32_t);
	check_condition(offset < dlen, false);

	h.protocols = *(uint8_t*)(buff + offset);
	offset += sizeof(uint8_t);
	check_condition(offset < dlen, false);

	h.reserved = *(uint32_t*)(buff + offset);
	offset += sizeof(uint32_t);
	check_condition(offset < dlen, false);

	h.bodylen = *(uint16_t*)(buff + offset);
	offset += sizeof(uint16_t);
	check_condition(offset <= dlen, false);
	return true;
}
