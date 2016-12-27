package Database::Async::SQLite 0.001;
# ABSTRACT: SQLite support for IO::Async

use strict;
use warnings;

use parent qw(DynaLoader);

__PACKAGE__->bootstrap(__PACKAGE__->VERSION);

1;
