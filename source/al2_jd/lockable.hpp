#pragma once

namespace apn::dark
{
	struct Lockable
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
	struct Locker {
		Lockable* p;
		Locker(Lockable* p) : p(p) { p->lock(); }
		~Locker() { p->unlock(); }
	};
}
