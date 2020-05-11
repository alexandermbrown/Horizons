#include "pch.h"
#include "Server.h"

#include <signal.h>
#include <time.h>

#include "yojimbo/yojimbo.h"

#include "Shared/Shared.h"

static volatile int quit = 0;

void interrupt_handler(int /*dummy*/)
{
    quit = 1;
}

int ServerMain()
{
    printf("started server on port %d (secure)\n", ServerPort);

    double time = 100.0;

    yojimbo::ClientServerConfig config;
    config.protocolId = ProtocolId;

    uint8_t privateKey[yojimbo::KeyBytes] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
                                     0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
                                     0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                     0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };

    yojimbo::Server server(yojimbo::GetDefaultAllocator(), privateKey, yojimbo::Address("127.0.0.1", ServerPort), config, adapter, time);

    server.Start(64);

    const double deltaTime = 0.1;

    char addressString[256];
    server.GetAddress().ToString(addressString, sizeof(addressString));
    printf("server address is %s\n", addressString);

    signal(SIGINT, interrupt_handler);

    while (!quit)
    {
        server.SendPackets();

        server.ReceivePackets();

        time += deltaTime;

        server.AdvanceTime(time);

        if (!server.IsRunning())
            break;

        yojimbo_sleep(deltaTime);
    }

    server.Stop();

    return 0;
}

int main()
{
    printf("\n");

    if (!InitializeYojimbo())
    {
        printf("error: failed to initialize Yojimbo!\n");
        return 1;
    }

    yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);

    srand((unsigned int)time(NULL));

    int result = ServerMain();

    ShutdownYojimbo();

    printf("\n");

    yojimbo::Matcher matcher(yojimbo::GetDefaultAllocator());
    matcher.RequestMatch();

    return result;
}