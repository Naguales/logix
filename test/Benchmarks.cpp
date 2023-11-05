#include "test/Benchmarks.h"
#include "Logix.h"
#include "logix/misc/scope_guard.hpp"

namespace test
{

	constexpr size_t messageCount = 100;

	void asyncTestWorker()
	{
		std::ostringstream ss;
		ss << "Greetings from the thread [" << std::this_thread::get_id() << "]!";
		std::string msg = ss.str();
		for (size_t i = 0; i < messageCount; ++i) {
			LOG_ERROR << msg;
			LOG_WARNING << msg;
			LOG_INFO << msg;
			LOG_DEBUG << msg;
			LOG_TRACE << msg;
		}
	}

	void test(bool async = true, bool multithreaded = true)
	{
		logix::defaultInitialization(async);

		auto onExit = sg::make_scope_guard([&]() {
			if (async)
				logix::finalize();
		});

		std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

		auto threadCount = std::thread::hardware_concurrency();
		if (multithreaded) {
			std::vector<std::shared_ptr<std::thread>> threads(threadCount);
			for (auto& thread : threads) {
				thread.reset(new std::thread(asyncTestWorker));
			}

			for (auto& thread : threads) {
				thread->join();
			}
		} else {
			for (size_t i = 0; i < threadCount; ++i) {
				asyncTestWorker();
			}
		}

		std::chrono::duration<double, std::milli> elapsedTimeMs = std::chrono::steady_clock::now() - start;
		constexpr size_t testedLogLevelCount = 5;
		double logMessageCount = testedLogLevelCount * messageCount * threadCount;
		LOG_INFO << fmt::format("Number of threads {}", threadCount);
		LOG_INFO << fmt::format("Number of log messages {}", logMessageCount);
		LOG_INFO << fmt::format("Elapsed time {} ms", elapsedTimeMs.count());
		LOG_INFO << fmt::format("Time per message logged {} us", (elapsedTimeMs.count() / logMessageCount) * 1e+3);
	}

	void asyncMultithreadedTest()
	{
		test();
	}

	void asyncOneThreadTest()
	{
		test(true, false);
	}

	void syncMultithreadedTest()
	{
		test(false);
	}

	void syncOneThreadTest()
	{
		test(false, false);
	}

} // namespace test
