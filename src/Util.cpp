#pragma once

float GetDistance3D(RE::NiPoint3* a_ni, const RE::NiPoint3& pos2)
{
	return sqrt(pow(a_ni->x - pos2.x, 2) + pow(a_ni->y - pos2.y, 2) + pow(a_ni->z - pos2.z, 2));
}

std::optional<RE::NiPoint3> FindNearestVertex(RE::TESObjectREFR* a_ref, const float minimum_offset)
{
	auto cell = a_ref->GetParentCell();

	if (!cell || !cell->navMeshes) {
		return std::nullopt;
	}

	auto& navMeshes = *cell->navMeshes;

	auto shortestDistance = FLT_MAX;

	std::optional<RE::NiPoint3> pos = std::nullopt;

	for (auto& navMesh : navMeshes.navMeshes) {
		for (auto& vertex : navMesh->vertices) {
			auto ni = a_ref->GetPosition();
			auto linearDistance = GetDistance3D(&ni, vertex.location);

			if (linearDistance < shortestDistance && linearDistance >= minimum_offset) {
				shortestDistance = linearDistance;
				pos.emplace(vertex.location);
			}
		}
	}

	return pos;
}

void MoveToPackageLocation(RE::Actor* a_actor, bool a_arg1)
{
	using func_t = decltype(&MoveToPackageLocation);
	REL::Relocation<func_t> func{ RELOCATION_ID(36472, 37471) };
	return func(a_actor, a_arg1);
}

void MoveTo_ImplMax(RE::TESObjectREFR* a_ref, const RE::ObjectRefHandle& a_targetHandle, RE::TESObjectCELL* a_targetCell, RE::TESWorldSpace* a_selfWorldSpace, const RE::NiPoint3& a_position, const RE::NiPoint3& a_rotation)
{
	using func_t = decltype(&MoveTo_ImplMax);
	REL::Relocation<func_t> func{ RE::Offset::TESObjectREFR::MoveTo };
	return func(a_ref, a_targetHandle, a_targetCell, a_selfWorldSpace, a_position, a_rotation);
}

bool MoveToNearestNavmesh(RE::TESObjectREFR* a_ref, const float minimum_offset)
{
	auto nearestVertex = FindNearestVertex(a_ref, minimum_offset);

	if (!nearestVertex) {
		_DMESSAGE("Not Vertex Found!");
		return false;
	}

	MoveTo_ImplMax(a_ref, a_ref->AsReference()->CreateRefHandle(), a_ref->GetParentCell(), a_ref->GetWorldspace(), std::move(*nearestVertex), a_ref->GetAngle());

	return true;
}
