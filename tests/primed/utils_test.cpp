#include "utils.hpp"

#include <gtest/gtest.h>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

struct ForkWrapper {
    ForkWrapper(const char * const env[]) {
        pid = fork();
        if (pid == 0) { //
            execle("/bin/sh", "/bin/sh", NULL, env);
        }
        usleep(100); // Wait for creation
    }

    ~ForkWrapper() {
        kill(pid, SIGKILL);
    }

    pid_t pid;
};

TEST(PrimedUtils, getEnvDict) {
    using namespace utils;

    { // 1. Empty
        char * const env[] = { NULL };

        ForkWrapper fw(env);

        EXPECT_EQ(EnvDict(), getEnvDict(fw.pid));
    }

    { // 2.
        const char * env[] = { "env1=val1", "env2=val2", NULL };
        EnvDict res = { { "env1", "val1" }, { "env2", "val2" } };

        ForkWrapper fw(env);

        EXPECT_EQ(res, getEnvDict(fw.pid));
    }

    { // 3.
        const char * env[] = { "env1=val1", "env2", "env3=", NULL };
        EnvDict res = { { "env1", "val1" }, { "env2", "" }, { "env3", "" } };

        ForkWrapper fw(env);

        EXPECT_EQ(res, getEnvDict(fw.pid));
    }

    { // 4.
        const char * env[] = { "env1==", "env2=a=", "env3==a", "env4===", NULL };
        EnvDict res = { { "env1", "=" }, { "env2", "a=" }, { "env3", "=a" }, {
                "env4", "==" } };

        ForkWrapper fw(env);

        EXPECT_EQ(res, getEnvDict(fw.pid));
    }
}
