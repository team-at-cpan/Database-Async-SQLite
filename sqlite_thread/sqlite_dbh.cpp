#include <stdexcept>
#include <iostream>
#include "sqlite_dbh.h"
#include <sqlite3.h>
#include <sys/eventfd.h>

#include "sqlite_sth.h"
#include "sqlite_queue.h"

sqlite_dbh::sqlite_dbh(
	const std::string &filename
):filename_{ filename },
  queue_{ new sqlite_queue { } }
{
	if(sqlite3_open_v2(
		filename_.data(),
		&db_,
		SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
		nullptr
	) != 0) {
		sqlite3_close(db_);
		throw std::runtime_error("failed");
	}
	fd_ = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE);
}

sqlite_dbh::~sqlite_dbh()
{
	sqlite3_close(db_);
}

sqlite_sth *
sqlite_dbh::prepare(const std::string &sql)
{
	return new sqlite_sth(*this, sql);
}

void
sqlite_dbh::push(sqlite_queue::item_type &&req)
{
	std::cerr << "Pushing new request\n";
	queue_->push(std::move(req));
	queue_->next();
}

