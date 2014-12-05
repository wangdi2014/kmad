#include "f_config.h"
#include "types.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>


namespace lcg = libconfig;

f_config::UsrFeatureMap f_config::ConfParser::parse_conf_file(
    const char* filename) {
  lcg::Config cnfg;
  try
  {
    cnfg.readFile(filename);
  }
  catch (const lcg::FileIOException &fioex) {
    std::cerr << "I/O error while reading file." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  catch (const lcg::ParseException &pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    std::exit(EXIT_FAILURE);
  }
  
  return process_config(cnfg);
}


f_config::UsrFeatureMap f_config::ConfParser::process_config(
    const lcg::Config& cnfg) {
  f_config::UsrFeatureMap feat_config;
  try
  {
    const lcg::Setting& root  = cnfg.getRoot();
    const lcg::Setting& features = root["feature_settings"]["usr_features"];
    int count = features.getLength();
    for (int i = 0; i < count; ++i) {
      FeatureSettings feat_set;
      const lcg::Setting& feature = features[i];
      std::string name;
      if (!feature.lookupValue("name", name)) 
        continue;
      feature.lookupValue("tag", feat_set.tag);
      bool found_add_score = feature.lookupValue("add_score", 
                                                 feat_set.add_score);
      bool found_sbtrct_score = feature.lookupValue("subtract_score",
                                                    feat_set.subtract_score);
      if (!(found_add_score || found_sbtrct_score))
        continue;

      //lcg::Setting& add_features_set = cnfg.lookup(name + ".add_features");
      lcg::Setting& add_features_set = feature["add_features"];
      for (int j = 0; j < add_features_set.getLength(); ++j) {
        feat_set.add_features.push_back(add_features_set[j]);
      }
      lcg::Setting& add_tags_set = feature["add_tags"];
      for (int j = 0; j < add_tags_set.getLength(); ++j) {
        feat_set.add_tags.push_back(add_tags_set[j]);
      }
      lcg::Setting& add_exceptions_set = feature["add_exceptions"];
      for (int j = 0; j < add_exceptions_set.getLength(); ++j) {
        feat_set.add_exceptions.push_back(add_exceptions_set[j]);
      }
      lcg::Setting& subtract_features_set = feature["subtract_features"];
      for (int j = 0; j < subtract_features_set.getLength(); ++j) {
        feat_set.subtract_features.push_back(subtract_features_set[j]);
      }
      lcg::Setting& subtract_tags_set = feature["subtract_tags"];
      for (int j = 0; j < subtract_tags_set.getLength(); ++j) {
        feat_set.subtract_tags.push_back(subtract_tags_set[j]);
      }
      lcg::Setting& subtract_exceptions_set = feature["subtract_exceptions"];
      for (int j = 0; j < subtract_exceptions_set.getLength(); ++j) {
        feat_set.subtract_exceptions.push_back(subtract_exceptions_set[j]);
      }
      lcg::Setting& positions_set = feature["positions"];
      for (int j = 0; j <  positions_set.getLength(); ++j) {
        FeaturePositions feat_pos; 
        positions_set[j].lookupValue("seq", feat_pos.seq);
        lcg::Setting& single_pos_set = positions_set[j]["pos"];
        for (int k = 0; k < single_pos_set.getLength(); ++k) {
          feat_pos.positions.push_back(single_pos_set[k]);
        }
        feat_set.positions.push_back(feat_pos);
      }
      feat_config[name] = feat_set;
    }
  }
  catch(const lcg::SettingNotFoundException &nfex)
  {
    std::cerr << "Setting not found" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return feat_config;
}