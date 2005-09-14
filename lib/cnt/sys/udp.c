/* This file is part of libcnt.
 *
 * Copyright (C) 2005 Cedric Cellier.
 *
 * Libcnt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * Libcnt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MicroModel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "config.h"
#include "libcnt/sys/udpi.h"
#include "libcnt/shelf.h"
#include "libcnt/hash.h"
#include "libcnt/shelf.h"
#include "libcnt/log.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
/* According to POSIX 1003.1-2001 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static cntHash *links = NULL;
static cntShelf *buffers = NULL;
static cntShelf *socks = NULL;

int udp_init(void) {
	if (links) {
		cntHash_clear(links);
	} else {
		links = cntHash_new(sizeof(udp_link), 5, 3, cntHash_BINKEYS, sizeof(struct sockaddr_in));
	}
	if (buffers) {
		cntShelf_clear(buffers);
	} else {
		buffers = cntShelf_new(sizeof(struct datagram), 8);
	}
	if (socks) {
		cntShelf_clear(socks);
	} else {
		socks = cntShelf_new(sizeof(udp_socket), 2);
	}
	if (!links || !buffers || !socks) return 0;
	return 1;
}

udp_socket *udp_socket_construct(udp_socket *this) {
	if (!this) {
		this = cntShelf_alloc(socks);
	}
	this->s = socket(PF_INET, SOCK_DGRAM, 0);
	if (-1 == this->s) {
		perror("socket");
		return NULL;
	}
	this->sent = this->received = this->nb_out = this->nb_in = 0;
	return this;
}

int udp_socket_bind(udp_socket *this, net_address *address) {
	assert(this);
	if ( 0!= bind(this->s, (struct sockaddr *)&address->s_in, sizeof(struct sockaddr)) ) {
		perror("bind");
		return 0;
	}
	return 1;
}

#include <sys/socket.h>
void udp_socket_destruct(udp_socket *this) {
	assert(this);
	if (0 != close(this->s)) {
		perror("close");
	}
}

void udp_socket_del(udp_socket *this) {
	assert(this);
	udp_socket_destruct(this);
	cntShelf_free(socks, this);
}

int udp_end(void) {
	cntHash_del(links);
	links = NULL;
	cntShelf_del(buffers);
	buffers = NULL;
	cntShelf_del(socks);
	socks = NULL;
	return 1;
}

udp_link *udp_link_construct(udp_link *this, net_address *address) {
	assert(links && buffers);
	if (!this) {
		log_warning(LOG_DEBUG, "Creating new link");
		cntHashkey key = { .bin = (unsigned char*)&address->s_in };
		this = cntHash_put(links, key, NULL);
	}
	memcpy(&this->address, address, sizeof(*address));
	this->first_datagram = this->last_datagram = NULL;
	this->sent = this->received = 0;
	this->nb_in = this->nb_out = 0;
	this->refcount = 1;
	return this;
}

void udp_link_del(udp_link *this) {
	assert(this);
	if (--this->refcount > 0) return;
	struct datagram *dtg = this->first_datagram;
	while (dtg) {
		struct datagram *next = dtg->next;
		cntShelf_free(buffers, dtg);
		dtg = next;
	}
	cntHashkey key = { .bin = &this->address.s_in };
	cntHash_remove(links, key);
}

int udp_socket_sendto(udp_socket *this, udp_link *link, unsigned len, const void *buf) {
	static char dummybuf = 0;
	assert(this && link && links && buffers);
	assert(0==len || buf);
	if (0==len) {	// because it's unclear to me wether it's allowed to send NULL to sendto in this case
		buf = &dummybuf;
	}
	if (-1 == sendto(this->s, buf, len, 0, (struct sockaddr *)&link->address, sizeof(struct sockaddr))) {
		perror("sendto");
		return 0;
	}
	link->sent += len;
	this->sent += len;
	link->nb_out ++;
	this->nb_out ++;
	return 1;
}

udp_link *udp_socket_receive(udp_socket *this, unsigned timeout) {
	assert(this && links && buffers);
	net_address from;
	socklen_t from_len = sizeof(from);
	struct datagram *dtg = NULL;
	size_t len =0;
	fd_set rd_set;
	struct timeval tv;
	do {
		FD_ZERO(&rd_set);
		FD_SET(this->s, &rd_set);
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		switch (select(this->s+1, &rd_set, NULL, NULL, &tv)) {
			case -1:
				perror("select");
				return 0;
			case 0:	// timeout
				return NULL;
			case EINTR:	// interrupted, continue
				break;
			default:
				dtg = cntShelf_alloc(buffers);
				len = recvfrom(this->s, dtg->recv_buf, UDP_MAX_SIZE, 0, (struct sockaddr *)&from.s_in, &from_len);
		}
	} while (! dtg);
	log_warning(LOG_DEBUG, "received datagram (len = %zd)", len);
	dtg->len = len;
	dtg->next = NULL;
	// identifier from, au besoin creer un udp_link, mettre à jour recv_buf, et retourner le link
	cntHashkey key = { .bin = (unsigned char *)&from.s_in };
	udp_link *link = cntHash_get(links, key);
	if (!link) {
		link = udp_link_construct(NULL, &from);
		if (!link) return 0;
	} else {
		log_warning(LOG_DEBUG, "reusing link at %p", link);
		link->refcount ++;
	}
	if (link->last_datagram) {
		link->last_datagram->next = dtg;
	} else {
		assert(!link->first_datagram);
		link->first_datagram = dtg;
	}
	link->last_datagram = dtg;
	link->received += len;
	this->received += len;
	link->nb_in ++;
	this->nb_in ++;
	return link;
}

struct datagram *udp_link_get_datagrams(udp_link *this) {
	assert(this);
	return this->first_datagram;
}

void udp_link_shift_datagrams(udp_link *this) {
	assert(this);
	assert(this->first_datagram && this->last_datagram);
	struct datagram *dtg = this->first_datagram;
	this->first_datagram = dtg->next;
	if (this->last_datagram == dtg) {
		this->last_datagram = NULL;
	}
	cntShelf_free(buffers, dtg);
}

net_address *udp_link_get_address(udp_link *this) {
	assert(this);
	return &this->address;
}

// vi:ts=3:sw=3
