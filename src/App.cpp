#include "test/Benchmarks.h"
#include "examples/Examples.h"
#include "fmt/format.h"

#include <iostream>

int main()
{
	fmt::print_colored(fmt::WHITE, "Choose the test to execute, and input the test number:\n\n");
	fmt::print_colored(fmt::GREEN, "1. The test for the asynchronous logger\n   {}",
		"utilizing the maximum supported number of concurrent threads as producers of log messages.\n");
	fmt::print_colored(fmt::GREEN, "2. The test for the asynchronous logger\n   {}",
		"utilizing one thread as a producer of log messages.\n");
	fmt::print_colored(fmt::CYAN, "3. The test for the synchronous logger\n   {}",
		"utilizing the maximum supported number of concurrent threads as producers of log messages.\n");
	fmt::print_colored(fmt::CYAN, "4. The test for the synchronous logger\n   {}",
		"utilizing one thread as a producer of log messages.\n");
	fmt::print_colored(fmt::YELLOW, "5. An example of the asynchronous logger usage.\n");
	fmt::print_colored(fmt::YELLOW, "6. An example of the synchronous logger usage.\n");
	int testNumber = 1;
	std::cin >> testNumber;

	switch (testNumber) {
	case 1:
		test::asyncMultithreadedTest();
		break;
	case 2:
		test::asyncOneThreadTest();
		break;
	case 3:
		test::syncMultithreadedTest();
		break;
	case 4:
		test::syncOneThreadTest();
		break;
	case 5:
		asyncLoggerUsageExample();
		break;
	case 6:
		syncLoggerUsageExample();
		break;
	default:
		test::asyncMultithreadedTest();
		break;
	}

	return 0;
}
