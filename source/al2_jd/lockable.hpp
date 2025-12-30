#pragma once

namespace apn::dark
{
	struct lockable_t
	{
		//
		// ロックカウントです。
		//
		int lock_count = 0;

		//
		// ロックされている場合はTRUEを返します。
		//
		BOOL is_locked() const { return lock_count != 0; }

		//
		// ロックします。
		//
		void lock() { lock_count++; }

		//
		// ロックを解除します。
		//
		void unlock() { lock_count--; }
	};

	//
	// このクラスはロック可能なオブジェクトをロックします。
	//
	struct locker_t {
		lockable_t* p;
		locker_t(lockable_t* p) : p(p) { p->lock(); }
		~locker_t() { p->unlock(); }
	};
}
