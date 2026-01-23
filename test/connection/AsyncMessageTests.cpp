#include "knx/connection/AsyncMessageState.h"
#include "knx/connection/KnxClientConnection.h"
#include <asio/steady_timer.hpp>
#include <gtest/gtest.h>

using namespace std::chrono_literals;

TEST(AsyncMessageTest, receiveFirstAttempt) {
  bool send = false;
  int numberOfSendsCalled = 0;
  asio::io_context ctx;
  knx::connection::CemiSendState *ref = nullptr;
  auto matcher = [&](auto& /*cemi*/) -> bool { return true; };
  auto sendResponse = [&]() -> asio::awaitable<void> {
    asio::steady_timer timer{ctx};
    timer.expires_after(7ms);
    co_await timer.async_wait();

    ref->setResponse({});
    co_return;
  };
  auto sender = [&]() -> asio::awaitable<void> {
    numberOfSendsCalled++;
    co_spawn(ctx, sendResponse(), asio::detached);
    co_return;
  };

  knx::connection::CemiSendState testObject{ctx, std::move(sender),
                                            std::move(matcher)};
  ref = &testObject;
  ASSERT_EQ(0, numberOfSendsCalled);

  asio::co_spawn(ctx, testObject.send(), asio::detached);
  ctx.run();

  ASSERT_EQ(knx::connection::CemiSendState::State::ok, testObject.getState());
  ASSERT_EQ(1, numberOfSendsCalled);
}

TEST(AsyncMessageTest, receiveSecondAttempt) {
  bool match = false;
  bool send = false;
  int numberOfSendsCalled = 0;
  asio::io_context ctx;
  knx::connection::CemiSendState *ref = nullptr;
  auto matcher = [&](auto & /*cemi*/) -> bool { return match; };
  auto sendResponse = [&]() -> asio::awaitable<void> {
    asio::steady_timer timer{ctx};
    timer.expires_after(7ms);
    co_await timer.async_wait();

    ref->setResponse({});
    co_return;
  };
  auto sender = [&]() -> asio::awaitable<void> {
    numberOfSendsCalled++;
    if (numberOfSendsCalled > 1) {
      match = true;
      co_spawn(ctx, sendResponse(), asio::detached);
    }
    co_return;
  };

  knx::connection::CemiSendState testObject{ctx, std::move(sender),
                                            std::move(matcher)};
  ref = &testObject;
  ASSERT_EQ(0, numberOfSendsCalled);

  asio::co_spawn(ctx, testObject.send(), asio::detached);
  ctx.run();

  ASSERT_EQ(knx::connection::CemiSendState::State::ok, testObject.getState());
  ASSERT_EQ(2, numberOfSendsCalled);
}

TEST(AsyncMessageTest, receiveSecondAttemptNoDelay) {
  bool match = false;
  bool send = false;
  int numberOfSendsCalled = 0;
  asio::io_context ctx;
  knx::connection::CemiSendState *ref = nullptr;
  auto matcher = [&](auto & /*cemi*/) -> bool { return match; };
  auto sender = [&]() -> asio::awaitable<void> {
    numberOfSendsCalled++;
    if (numberOfSendsCalled > 1) {
      match = true;
      // simulate co_routine imiedatly receives reponse
      ref->setResponse({});
    }
    co_return;
  };

  knx::connection::CemiSendState testObject{ctx, std::move(sender),
                                            std::move(matcher)};
  ref = &testObject;
  ASSERT_EQ(0, numberOfSendsCalled);

  asio::co_spawn(ctx, testObject.send(), asio::detached);
  ctx.run();

  ASSERT_EQ(knx::connection::CemiSendState::State::ok, testObject.getState());
  ASSERT_EQ(2, numberOfSendsCalled);
}

TEST(AsyncMessageTest, timeout) {
  bool match = false;
  bool send = false;
  int numberOfSendsCalled = 0;
  asio::io_context ctx;
  knx::connection::CemiSendState *ref = nullptr;
  auto matcher = [&](auto & /*cemi*/) -> bool { return match; };
  auto sender = [&]() -> asio::awaitable<void> {
    numberOfSendsCalled++;
    co_return;
  };

  knx::connection::CemiSendState testObject{ctx, std::move(sender),
                                            std::move(matcher)};
  ref = &testObject;
  ASSERT_EQ(0, numberOfSendsCalled);

  asio::co_spawn(ctx, testObject.send(), asio::detached);
  ctx.run();

  ASSERT_EQ(knx::connection::CemiSendState::State::timeout, testObject.getState());
  ASSERT_EQ(2, numberOfSendsCalled);
}
