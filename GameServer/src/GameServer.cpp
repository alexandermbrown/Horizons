#include "pch.h"
#include "GameServer.h"

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
    printf("started server on port %d (insecure)\n", ServerPort);

    double time = 50.0; // 20hz

    yojimbo::ClientServerConfig config;

    uint8_t privateKey[yojimbo::KeyBytes];
    memset(privateKey, 0, yojimbo::KeyBytes);

    yojimbo::Server server(yojimbo::GetDefaultAllocator(), privateKey, yojimbo::Address("127.0.0.1", ServerPort), config, adapter, time);

    server.Start(64);

    char addressString[256];
    server.GetAddress().ToString(addressString, sizeof(addressString));
    printf("server address is %s\n", addressString);

    const double deltaTime = 0.005f;

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

    return result;
}