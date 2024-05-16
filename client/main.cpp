#include <iostream>
#include <enet/enet.h>
#include <thread>

int main()
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost* client;

	client = enet_host_create(NULL, 1, 2, 0, 0);

	if (nullptr == client)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	bool gameLoop = true;
	while (gameLoop)
	{
		ENetEvent event;

		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
				event.peer->data = "Client information";
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);

				event.peer->data = NULL;
			}
		}

		// Do game codes
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	enet_host_destroy(client);
	return EXIT_SUCCESS;
}