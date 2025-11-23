
#include <gtest/gtest.h>
#include <lock.h>


TEST(TestUniqueLock, test_lock_unlock) {
    using mutex_type = framework::spin_lock;
    using lock_type = framework::unique_lock<mutex_type>;

    mutex_type mutex;

    {
        lock_type lock(mutex);
        EXPECT_TRUE(static_cast<bool>(lock));
        EXPECT_TRUE(lock.owns());

        lock.unlock();
        EXPECT_FALSE(static_cast<bool>(lock));
        EXPECT_FALSE(lock.owns());

        lock.lock();
        EXPECT_TRUE(static_cast<bool>(lock));
        EXPECT_TRUE(lock.owns());
    }
}
