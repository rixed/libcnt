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
#include "libcnt/sys/neti.h"
#include <assert.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

net_address *net_address_construct(net_address *this, char *ip, unsigned port) {
	assert(this && ip);
	struct sockaddr_in *this_ = &this->s_in;
	memset(this_, 0, sizeof(*this_));	// zero the padding bytes because we may use this net_adress as a hashkey
	this_->sin_family=AF_INET;
	this_->sin_port=htons(port);
	if (!inet_aton(ip, &this_->sin_addr)) {
		return NULL;
	}
	memset(&this_->sin_zero, 0, 8);
	return this;
}

// vi:ts=3:sw=3
