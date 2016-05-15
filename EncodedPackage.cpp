#include "EncodedPackage.h"


EncodedPackage::EncodedPackage()
{
}

EncodedPackage::~EncodedPackage()
{
}

bool
operator==(const EncodedPackage& lep, const EncodedPackage& rep) {
	return lep.index == rep.index &&
		   lep.d == rep.d &&
		   lep.adjacency == rep.adjacency;
}

bool
operator!=(const EncodedPackage& lep, const EncodedPackage& rep) {
	return !(lep == rep);
}

bool
operator<(const EncodedPackage& lep, const EncodedPackage& rep) {
	return lep.d < rep.d || lep.adjacency < rep.adjacency;
}

bool
operator>(const EncodedPackage& lep, const EncodedPackage& rep) {
	return lep.d > rep.d;
}

ostream&
operator<<(ostream& os, const EncodedPackage& ep) {
    os << ep.index << " " << ep.d;
	for (int d: ep.adjacency) {
		os << " " << d;
	}
	return os;
}

int
EncodedPackage::getd() {
	return d;
}

int
EncodedPackage::getindex()
{
	return index;
}
