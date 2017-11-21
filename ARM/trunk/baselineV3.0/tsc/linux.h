#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <linux/rtc.h>
#include <ctime>
#include <cstdlib>
//#include <linux/pwm.h>

namespace hik
{
	class watchdog
	{
	private:
		int fd;
	public:
		watchdog(bool enablewdt = false)
		{
			if (enablewdt)
				enable();
			else
				fd = -1;
		}
		~watchdog()
		{
			disable();
		}

		void enable(int timeout = 3)
		{
			if (fd == -1)
			{
				fd = open("/dev/watchdog", O_RDWR);
				//ioctl(fd, WDIOS_ENABLECARD);
			}
			ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
		}
		
		void disable()
		{
			if (fd != -1)
			{
				//ioctl(fd, WDIOS_DISABLECARD);
				close(fd);
				fd = -1;
			}
		}

		void feed()
		{
			if (fd != -1)
				ioctl(fd, WDIOC_KEEPALIVE);
		}

		void sysctl()
		{
			disable();
			std::system("watchdog -t 1 -T 3 /dev/watchdog");
		}

		void appctl()
		{
			std::system("kill -9 `ps | sed -n '/watchdog$/p' | awk '{print $1}'`");
			enable();
		}
	};

	class rtc
	{
	private:
		int fd;
	public:
		rtc(const char *devname = "/dev/rtc")
		{
			fd = open(devname, O_RDWR);
		}
		~rtc()
		{
			if (fd != -1)
			{
				close(fd);
				fd = -1;
			}
		}
		void set(const std::tm &t)
		{
			if (fd == -1)
				return;
			struct rtc_time r;
			r.tm_sec = t.tm_sec;
			r.tm_min = t.tm_min;
			r.tm_hour = t.tm_hour;
			r.tm_mday = t.tm_mday;
			r.tm_mon = t.tm_mon;
			r.tm_year = t.tm_year;
			r.tm_wday = t.tm_wday;
			r.tm_yday = t.tm_yday;
			r.tm_isdst = t.tm_isdst;
			ioctl(fd, RTC_SET_TIME, &r);
		}

		void set(std::time_t t)
		{
			std::tm tm;
			localtime_r(&t, &tm);
			set(tm);
		}

		std::time_t get()
		{
			if (fd == -1)
				return std::time(NULL);
			std::tm r;
			struct rtc_time t;
			ioctl(fd, RTC_RD_TIME, &t);
			r.tm_sec = t.tm_sec;
			r.tm_min = t.tm_min;
			r.tm_hour = t.tm_hour;
			r.tm_mday = t.tm_mday;
			r.tm_mon = t.tm_mon;
			r.tm_year = t.tm_year;
			r.tm_wday = t.tm_wday;
			r.tm_yday = t.tm_yday;
			r.tm_isdst = t.tm_isdst;
			return std::mktime(&r);
		}
	};

	static inline bool dir_exist(const char *path)
	{
		if (path == nullptr)
			return false;
		struct stat st;
		if (stat(path, &st) == -1)
			return false;
		return S_ISDIR(st.st_mode) > 0;
	}
}
