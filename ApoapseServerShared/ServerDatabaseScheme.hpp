#pragma once
#include "TypeDefs.hpp"

static std::string GetServerDbScheme()
{
	static std::string scheme = R"(
{
	"tables":
	[
		{
			"name": "operations",
			"fields":
			[
				{
					"name": "id",
					"type": "integer",
					"primary": true
				},		
				{
					"name": "type",
					"type": "blob"
				},
				{
					"name": "direction",
					"type": "blob"
				},
				{
					"name": "time",
					"type": "integer"
				},
				{
					"name": "user",
					"type": "blob"
				},
				{
					"name": "item_uuid",
					"type": "blob",
					"canBeNull": true
				}
			]
		},
		{
			"name": "users",
			"fields":
			[
				{
					"name": "user_id",
					"type": "integer",
					"primary": true
				},		
				{
					"name": "username_hash",
					"type": "blob",
					"unique": true
				},
				{
					"name": "password",
					"type": "blob"
				},
				{
					"name": "password_salt",
					"type": "blob"
				},
				{
					"name": "is_temporary_passsword",
					"type": "int"
				}
			]
		},
		{
			"name": "rooms",
			"fields":
			[
				{
					"name": "id",
					"type": "integer",
					"primary": true
				},		
				{
					"name": "uuid",
					"type": "blob",
					"unique": true
				}
			]
		},
		{
			"name": "threads",
			"fields":
			[
				{
					"name": "id",
					"type": "integer",
					"primary": true
				},
				{
					"name": "room_uuid",
					"type": "blob"
				},	
				{
					"name": "uuid",
					"type": "blob",
					"unique": true
				}
			]
		},
		{
			"name": "messages",
			"fields":
			[
				{
					"name": "id",
					"type": "integer",
					"primary": true
				},
				{
					"name": "uuid",
					"type": "blob",
					"unique": true
				},		
				{
					"name": "thread_uuid",
					"type": "blob"
				},
				{
					"name": "author",
					"type": "blob"
				},
				{
					"name": "sent_time",
					"type": "text"
				},
				{
					"name": "content",
					"type": "blob"
				}
			]
		}
	]
}
	)";

	return scheme;
}