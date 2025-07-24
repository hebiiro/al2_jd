#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはパレットです。ピグメントを管理します。
	//
	struct Palette
	{
		//
		// ピグメントのコレクションです。
		//
		std::unordered_map<std::pair<int, int>, Pigment> pigments;

		//
		// ピグメントを返します。
		//
		const Pigment* get(int part_id, int state_id) const
		{
			if (auto it = pigments.find({ part_id, state_id }); it != pigments.end())
				return &it->second;
			else
				return {};
		}

		//
		// ピグメントを追加します。
		//
		void set(int part_id, int state_id, Pigment pigment)
		{
			pigments[{ part_id, state_id }] = pigment;
		}
	};
}
