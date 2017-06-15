#include "generator/region_meta.hpp"

#include "platform/platform.hpp"

#include "coding/file_container.hpp"
#include "coding/file_name_utils.hpp"

#include "defines.hpp"

int main(int argc, char ** argv)
{
  if (argc <= 1)
  {
    LOG(LERROR, ("Usage:", argc == 1 ? argv[0] : "update_region_data",
                 "<mwm> [<data_path>]"));
    return 1;
  }

  Platform & pl = GetPlatform();
  if (argc > 2)
    pl.SetResourceDir(argv[2]);

  feature::RegionData regionData;
  if (!feature::ReadRegionData(my::FilenameWithoutExt(argv[1]), regionData))
  {
    LOG(LERROR, ("No extra data for country:", my::FilenameWithoutExt(argv[1])));
    return 2;
  }

  FilesContainerW writer(argv[1], FileWriter::OP_WRITE_EXISTING);
  FileWriter w = writer.GetWriter(REGION_INFO_FILE_TAG);
  regionData.Serialize(w);

  return 0;
}
