#pragma once

namespace test
{

	/**
	 * The test for the asynchronous logger
	 * utilizing the maximum supported number of concurrent threads as producers of log messages.
	 */
	void asyncMultithreadedTest();

	/**
	 * The test for the asynchronous logger
	 * utilizing one thread as a producer of log messages.
	 */
	void asyncOneThreadTest();

	/**
	 * The test for the synchronous logger
	 * utilizing the maximum supported number of concurrent threads as producers of log messages.
	 */
	void syncMultithreadedTest();

	/**
	 * The test for the asynchronous logger
	 * utilizing one thread as a producer of log messages.
	 */
	void syncOneThreadTest();

} // namespace test
