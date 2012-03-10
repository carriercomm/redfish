/*
 * Copyright 2012 the Redfish authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef REDFISH_UTIL_ENDPOINT_DOT_H
#define REDFISH_UTIL_ENDPOINT_DOT_H

#include <stdint.h>

#include "util/compiler.h"

struct endpoint {
	uint32_t ip;
	uint16_t port;
};
PACKED(
struct packed_endpoint {
	uint32_t ip;
	uint16_t port;
});

#endif
