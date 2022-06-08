#pragma once

float GetDistance3D(RE::NiPoint3* a_ni, const RE::NiPoint3& pos2);

std::optional<RE::NiPoint3> FindNearestVertex(RE::TESObjectREFR* a_ref, const float minimum_offset = 0.0f);

void MoveToPackageLocation(RE::Actor* a_actor, bool a_arg1 = false);
void MoveTo_ImplMax(RE::TESObjectREFR* a_ref, const RE::ObjectRefHandle& a_targetHandle, RE::TESObjectCELL* a_targetCell, RE::TESWorldSpace* a_selfWorldSpace, const RE::NiPoint3& a_position, const RE::NiPoint3& a_rotation);
bool MoveToNearestNavmesh(RE::TESObjectREFR* a_ref, const float minimum_offset = 0.f);
bool MoveToRef(RE::TESObjectREFR* a_actor, RE::TESObjectREFR* a_target);

bool IsPersistent(RE::TESObjectREFR* a_ref);
bool IsPersistent(const RE::TESObjectREFR* a_ref);
