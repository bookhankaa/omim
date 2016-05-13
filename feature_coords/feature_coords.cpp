#include "base/logging.hpp"

#include "coding/file_name_utils.hpp"

#include "geometry/mercator.hpp"
#include "geometry/point2d.hpp"

#include "indexer/classificator.hpp"
#include "indexer/classificator_loader.hpp"
#include "indexer/feature.hpp"
#include "indexer/feature_processor.hpp"
#include "indexer/map_style_reader.hpp"

#include "platform/platform.hpp"

#include "std/iostream.hpp"

struct Stats
{
  uint32_t m_count;
};

class ClosestPoint
{
  m2::PointD const & m_center;
  m2::PointD m_best;
  double m_distance = -1;

public:
  ClosestPoint(m2::PointD const & center) : m_center(center) {}

  m2::PointD GetBest() const { return m_best; }

  void operator() (m2::PointD const & point)
  {
    double distance = m_center.SquareLength(point);
    if (m_distance < 0 || distance < m_distance)
    {
      m_distance = distance;
      m_best = point;
    }
  }
};

bool IsNeeded(FeatureType const & f)
{
  if (!f.GetHouseNumber().empty())
    return true;
  // Get classificator types and return ok for true POIs
  bool foundPOI = false;
  f.ForEachType([&foundPOI](uint32_t type)
  {
    string fullName = classif().GetFullObjectName(type);
    auto pos = fullName.find("|");
    if (pos != string::npos)
      fullName = fullName.substr(0, pos);
    if (fullName == "amenity" || fullName == "shop" || fullName == "tourism")
      foundPOI = true;
  });
  return foundPOI;
}

class Processor
{
  Stats & m_stats;

public:
  Processor(Stats & stats) : m_stats(stats) {}

  void operator() (FeatureType const & f, uint32_t const & id)
  {
    f.ParseBeforeStatistic();
    if (!IsNeeded(f))
      return;
    ClosestPoint closest(f.GetLimitRect(FeatureType::BEST_GEOMETRY).Center());
    if (f.GetFeatureType() == feature::GEOM_AREA)
    {
      f.ForEachTriangle([&closest](m2::PointD const & p1, m2::PointD const & p2, m2::PointD const & p3)
      {
        closest((p1 + p2 + p3) / 3);
      }, FeatureType::BEST_GEOMETRY);
    }
    else
    {
      f.ForEachPoint(closest, FeatureType::BEST_GEOMETRY);
    }
    ms::LatLon ll = MercatorBounds::ToLatLon(closest.GetBest());
    cout << id << "," << f.GetFeatureType() << "," << ll.lon << "," << ll.lat << endl;
    m_stats.m_count++;
  }
};

int main(int argc, char ** argv)
{
  if (argc <= 1)
  {
    LOG(LERROR, ("Usage:", argc == 1 ? argv[0] : "feature_coords", "<mwm_file_name> [<data_path>]"));
    return 1;
  }

  Platform & pl = GetPlatform();

  if (argc > 2)
    pl.SetResourceDir(argv[2]);

  GetStyleReader().SetCurrentStyle(MapStyleMerged);
  classificator::Load();
  classif().SortClassificator();

  Stats info;
  info.m_count = 0;
  Processor doProcess(info);
  feature::ForEachFromDat(argv[1], doProcess);

  cout << endl << info.m_count << endl;
  return 0;
}
