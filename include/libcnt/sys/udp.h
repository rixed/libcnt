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
#ifndef UDP_H_040710
#define UDP_H_040710
#include <stdlib.h>
#include <libcnt/sys/net.h>

typedef struct s_udp_socket udp_socket;
typedef struct s_udp_link udp_link;

#define UDP_MAX_SIZE 60000	// rude
struct datagram {
	struct datagram *next;
	unsigned len;
	unsigned char recv_buf[UDP_MAX_SIZE];
};

int udp_init(void);
int udp_end(void);
udp_socket *udp_socket_construct(udp_socket *this);
static inline udp_socket *udp_socket_new(void) { return udp_socket_construct(NULL); }
void udp_socket_destruct(udp_socket *this);
void udp_socket_del(udp_socket *this);
int udp_socket_bind(udp_socket *this, net_address *address);
udp_link *udp_link_construct(udp_link *this, net_address *address);
void udp_link_del(udp_link *this);
/** send a datagram to a link
 * len is in bytes
 * returns 0 on errors
 * (if len==0, buf wont be read)
 */
int udp_socket_sendto(udp_socket *this, udp_link *link, unsigned len, const void *buf);
udp_link *udp_socket_receive(udp_socket *this, unsigned timeout /* in seconds */);
struct datagram *udp_link_get_datagrams(udp_link *this);
/** remove first datagram from datagrams chain
 */
void udp_link_shift_datagrams(udp_link *this);
net_address *udp_link_get_address(udp_link *this);

#endif
// vi:ts=3:sw=3
