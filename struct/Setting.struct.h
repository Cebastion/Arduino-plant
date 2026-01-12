#pragma once
#include "./PointWiFi.struct.h"
#include <vector>

using namespace std;

struct Setting {
  vector<PointWiFi> WiFis;
  size_t length;
};
