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
#ifndef PRIV_UDP_040710
#define PRIV_UDP_040710

#include <libcnt/sys/neti.h>

struct s_udp_link {
	net_address address;
	struct datagram *first_datagram, *last_datagram;
	unsigned sent, received;
	unsigned nb_out, nb_in;
	unsigned refcount;
};

struct s_udp_socket {
	int s;
	unsigned sent, received;
	unsigned nb_out, nb_in;
};

#include <libcnt/sys/udp.h>
#endif
// vi:ts=3:sw=3
