#define BOOST_TEST_DYN_LINK

#include "fasta.h"
#include "f_config.h"
#include "msa.h"
#include "profile.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <cmath>


BOOST_AUTO_TEST_SUITE(test_msa)

BOOST_AUTO_TEST_CASE(test_run_msa)
{
  int codon_length = 7;
  fasta::Sequence s1;
  // AKLCAKL
  s1 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "SAAAZAA"
                                 "YAAAAAAAAAAAAAKAAAAAA"
                                 "LAAAAAA", codon_length);
  fasta::Sequence s2;
  // AKLAKLR
  s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "YAAAZAA"
                                 "AAAAAAAKAAAAAALAAAAAA"
                                 "RAAAAAA", codon_length);
  FeatureNamesList feature_list = {"ptm_phosph0", "ptm_phosph1",
                                   "ptm_phosph2", "ptm_phosph3",
                                   "ptm_phosphP", "ptm_acet0",
                                   "ptm_acet1", "ptm_acet2",
                                   "ptm_acet3", "ptm_Nglyc0",
                                   "ptm_Nglyc1", "ptm_Nglyc2",
                                   "ptm_Nglyc3", "ptm_amid0",
                                   "ptm_amid1", "ptm_amid2",
                                   "ptm_amid3", "ptm_hydroxy0",
                                   "ptm_hydroxy1", "ptm_hydroxy2",
                                   "ptm_hydroxy3", "ptm_methyl0",
                                   "ptm_methyl1", "ptm_methyl2",
                                   "ptm_methyl3", "ptm_Oglyc0",
                                   "ptm_Oglyc1", "ptm_Oglyc2",
                                   "ptm_Oglyc3"}; 
  std::map<std::string, double> probabilities;
  f_config::FeatureSettingsMap f_set;
  fasta::SequenceList sequences = {s1, s2};
  int domain_modifier = 4;
  int motif_modifier = 3;
  int ptm_modifier = 10;
  double gap_open_pen = -5;
  double gap_ext_pen = -1;
  double end_pen = -1;
  bool one_round = false;
  seq_data::SequenceData sequence_data;
  sequence_data.sequences = sequences;
  sequence_data.feature_list = feature_list;
  std::vector<fasta::SequenceList> alignment;
  std::string sbst_mat = "BLOSUM";
  bool first_gapped = false;
  bool optimize = false;
  alignment = msa::run_msa(sequence_data, f_set, gap_open_pen, gap_ext_pen,
                           end_pen, domain_modifier, motif_modifier, 
                           ptm_modifier, codon_length, one_round, sbst_mat,
                           first_gapped, optimize);
  fasta::Sequence e_s1;
  // AKLCAKL
  e_s1 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                   "SAAAZAA"
                                   "YAAAAAAAAAAAAAKAAAAAA"
                                   "LAAAAAA", codon_length);
  fasta::Sequence e_s2;
  // AKLAKL
  e_s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                   "YAAAZAA"
                                   "-AAAAAAAAAAAAAKAAAAAA"
                                   "LAAAAAA", codon_length);
  fasta::Sequence e_s1_lower;
  // AKLCAKL
  e_s1_lower = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                         "SAAAZAA"
                                         "YAAAAAAAAAAAAAKAAAAAA"
                                         "LAAAAAA", codon_length);
  fasta::Sequence e_s2_lower;
  // AKLAKL
  e_s2_lower = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                         "YAAAZAA"
                                         "-AAAAAAAAAAAAAKAAAAAA"
                                         "lAAAAAA", codon_length);
  std::vector<fasta::SequenceList> expected_alignment = {{e_s1, e_s2},
                                                         {e_s1_lower,
                                                          e_s2_lower}};
  BOOST_CHECK_EQUAL(alignment[0][0].residues.size(),
                    expected_alignment[0][0].residues.size());
  BOOST_CHECK_EQUAL(alignment[0][1].residues.size(),
                    expected_alignment[0][1].residues.size());
  BOOST_CHECK_EQUAL(alignment[1][0].residues.size(),
                    expected_alignment[1][0].residues.size());
  BOOST_CHECK_EQUAL(alignment[1][1].residues.size(),
                    expected_alignment[1][1].residues.size());

  for (size_t i = 0; i < alignment[0][0].residues.size(); ++i) {
    BOOST_CHECK_EQUAL(alignment[0][0].residues[i].codon,
                      expected_alignment[0][0].residues[i].codon);
  }
  for (size_t i = 0; i < alignment[0][1].residues.size(); ++i) {
    BOOST_CHECK_EQUAL(alignment[0][1].residues[i].codon,
                      expected_alignment[0][1].residues[i].codon);
  }
  for (size_t i = 0; i < alignment[1][0].residues.size(); ++i) {
    BOOST_CHECK_EQUAL(alignment[1][0].residues[i].codon,
                      expected_alignment[1][0].residues[i].codon);
  }
  for (size_t i = 0; i < alignment[1][1].residues.size(); ++i) {
    BOOST_CHECK_EQUAL(alignment[1][1].residues[i].codon,
                      expected_alignment[1][1].residues[i].codon);
  }
}

BOOST_AUTO_TEST_CASE(test_run_msa_gapped_mode)
{
  f_config::FeatureSettingsMap f_set;
  fasta::SequenceList sequences;
  sequences = {fasta::make_sequence("WWTWW", 1),
               fasta::make_sequence("WTWRW", 1),
               fasta::make_sequence("WRWTWRW", 1)};
  int domain_modifier = 0;
  int motif_modifier = 0;
  int ptm_modifier = 0;
  double gap_open_pen = -4;
  double gap_ext_pen = -4;
  double end_pen = -4;
  int codon_length = 1;
  bool one_round = false;
  seq_data::SequenceData sequence_data;
  FeatureNamesList feature_list;
  std::map<std::string, double> probabilities;
  sequence_data.sequences = sequences;
  sequence_data.feature_list = feature_list;
  std::vector<fasta::SequenceList> alignment;
  std::string sbst_mat = "BLOSUM";
  bool first_gapped = true;
  bool optimize = false;
  alignment = msa::run_msa(sequence_data, f_set, gap_open_pen, gap_ext_pen,
                           end_pen, domain_modifier, motif_modifier, 
                           ptm_modifier, codon_length, one_round, sbst_mat,
                           first_gapped, optimize);

  BOOST_CHECK_EQUAL(alignment.size(), 2);
  BOOST_CHECK_EQUAL(alignment[0].size(), 3);
  BOOST_CHECK_EQUAL(alignment[0].size(), alignment[1].size());

  std::vector<std::string> result;
  for (auto& seqpair : alignment) {
    for (auto& seq : seqpair) {
      result.push_back(fasta::make_string(seq));
    }
  }
  std::vector<std::string> expected = {"W-WTW-W",
                                       "--WTWRW",
                                       "WRWTWRW",
                                       "W-WTW-W",
                                       "--WTWRW",
                                       "WRWTWRW"};

  BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                expected.begin(), expected.end());
}
BOOST_AUTO_TEST_CASE(test_set_identities)
{
  int codon_length = 7;
  fasta::Sequence s1;
  // AKLCAKL
  s1 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "SAAAZAA"
                                 "YAAAAAAAAAAAAAKAAAAAA"
                                 "LAAAAAA", codon_length);
  fasta::Sequence s2;
  // AKLAKL
  s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "YAAAZAA"
                                 "AAAAAAAKAAAAAALAAAAAA"
                                 "RAAAAAA", codon_length);
  FeatureNamesList feature_list = {"ptm_phosph0", "ptm_phosph1",
                                   "ptm_phosph2", "ptm_phosph3",
                                   "ptm_phosphP", "ptm_acet0",
                                   "ptm_acet1", "ptm_acet2",
                                   "ptm_acet3", "ptm_Nglyc0",
                                   "ptm_Nglyc1", "ptm_Nglyc2",
                                   "ptm_Nglyc3", "ptm_amid0",
                                   "ptm_amid1", "ptm_amid2",
                                   "ptm_amid3", "ptm_hydroxy0",
                                   "ptm_hydroxy1", "ptm_hydroxy2",
                                   "ptm_hydroxy3", "ptm_methyl0",
                                   "ptm_methyl1", "ptm_methyl2",
                                   "ptm_methyl3", "ptm_Oglyc0",
                                   "ptm_Oglyc1", "ptm_Oglyc2",
                                   "ptm_Oglyc3"}; 
  f_config::FeatureSettingsMap f_set;
  double gap_open_pen = -5;
  double gap_ext_pen = -1;
  double end_pen = -1;
  int domain_modifier = 4;
  int motif_modifier = 3;
  int ptm_modifier = 10;
  fasta::SequenceList sequences = {s1, s2};
  seq_data::SequenceData sequence_data;
  sequence_data.sequences = sequences;
  sequence_data.feature_list = feature_list;
  FeatureScores f_profile(sequence_data.feature_list, domain_modifier,
                          ptm_modifier, motif_modifier,
                          sequence_data.probabilities);
  // query_seq_list - the profile are built only based on the first
  // sequence
  fasta::SequenceList query_seq_list = {s1};
  std::string sbst_mat = "BLOSUM";
  profile::ProfileMap profile = profile::create_score_profile(
      query_seq_list, sbst_mat);
  f_profile.update_scores(query_seq_list, f_set);
  std::vector<double> result_identities = msa::set_identities(sequence_data,
                                                              profile,
                                                              f_profile,
                                                              gap_open_pen,
                                                              end_pen, 
                                                              gap_ext_pen,
                                                              codon_length);
  std::vector<double> expected_identities = {1, 0.857};
  BOOST_CHECK_EQUAL(expected_identities.size(), result_identities.size());
  for (size_t i = 0; i < result_identities.size(); ++i) {
    BOOST_CHECK(std::abs(result_identities[i] - expected_identities[i]
                          < 0.0001));
  }
}

BOOST_AUTO_TEST_CASE(test_calc_identity) {
  int codon_length = 7;
  fasta::Sequence dummy;
  // -AAAAAAAAA
  dummy = fasta::make_sequence("d", "-AAAAAAAAAAAAAAAAAAAA"
                                 "AAAAAAAAAAAAAAAAAAAAA"
                                 "AAAAAAAAAAAAAAAAAAAAA"
                                 "AAAAAAA", codon_length);
  fasta::Sequence query;
  // KLDDDDAKL
  query = fasta::make_sequence("d", "KAAAAAALAAAAAA"
                                 "DAAAAAADAAAAAADAAAAAA"
                                 "DAAAAAAAAAAAAAKAAAAAA"
                                 "LAAAAAA", codon_length);
  fasta::Sequence s2;
  // AKLN---AKL
  s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "NAAAAAA-AAAAAA-AAAAAA"
                                 "-AAAAAAAAAAAAAKAAAAAA"
                                 "LAAAAAA", codon_length);
  double result_identity = msa::calc_identity(dummy, s2, query);
  double expected_identity = 0.5;
  BOOST_CHECK_EQUAL(result_identity, expected_identity);

}

BOOST_AUTO_TEST_CASE(test_remove_gaps) {
  int codon_length = 7;
  fasta::Sequence s1;
  // AKLDDDDA-KL-
  s1 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "DAAAAAADAAAAAADAAAAAA"
                                 "DAAAAAAAAAAAAA-AAAAAA"
                                 "KAAAAAALAAAAaa-AAAAAA", codon_length);
  fasta::Sequence s2;
  // AKLN---ADKLD
  s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                 "NAAAAAA-AAAAAA-AAAAAA"
                                 "-AAAAAAAAAAAAADAAAAAA"
                                 "KAAAAaaLAAAAAADAAAAAA", codon_length);
  fasta::SequenceList sequences = {s1, s2};
  fasta::SequenceList result_sequences = msa::remove_gaps(sequences);
  fasta::Sequence e_s2;
  fasta::Sequence e_s2_lower;
  // AKLN---akl
  e_s2 = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                   "NAAAAAA-AAAAAA-AAAAAA"
                                   "-AAAAAAAAAAAAAKAAAAaa"
                                   "LAAAAAA", codon_length);
  e_s2_lower = fasta::make_sequence("d", "AAAAAAAKAAAAAALAAAAAA"
                                         "NAAAAAA-AAAAAA-AAAAAA"
                                         "-AAAAAAaAAAAAAkAAAAaa"
                                         "lAAAAAA", codon_length);
  fasta::SequenceList expected_sequences = {e_s2, e_s2_lower};

  BOOST_CHECK_EQUAL(result_sequences[0].residues.size(),
                    expected_sequences[0].residues.size());
  BOOST_CHECK_EQUAL(result_sequences[1].residues.size(),
                    expected_sequences[1].residues.size());
  BOOST_CHECK_EQUAL(result_sequences[0].residues.size(),
                    result_sequences[1].residues.size());
  for (size_t i = 0; i < result_sequences[0].residues.size(); ++i) {
    BOOST_CHECK_EQUAL(result_sequences[0].residues[i].codon,
                      expected_sequences[0].residues[i].codon);
    BOOST_CHECK_EQUAL(result_sequences[1].residues[i].codon,
                      expected_sequences[1].residues[i].codon);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        result_sequences[0].residues[i].features.begin(),
        result_sequences[0].residues[i].features.end(),
        expected_sequences[0].residues[i].features.begin(),
        expected_sequences[0].residues[i].features.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(
        result_sequences[1].residues[i].features.begin(),
        result_sequences[1].residues[i].features.end(),
        expected_sequences[1].residues[i].features.begin(),
        expected_sequences[1].residues[i].features.end());
  }
}

BOOST_AUTO_TEST_CASE(test_count_alignments) {
 std::vector<double> identities = {0.1, 0.2, 0.3, 0.9, 0.6, 0.7, 0.3, 1.};
 double cutoff = 0.5;
 int result = msa::count_alignments(cutoff, identities);
  BOOST_CHECK_EQUAL(result, 4);
}

BOOST_AUTO_TEST_CASE(test_add_alignment) {
  fasta::Sequence prof1 = fasta::make_sequence("A-A-", 1);
  fasta::Sequence s1 = fasta::make_sequence("ABAD", 1);
  fasta::Sequence prof2 = fasta::make_sequence("A---A", 1);
  fasta::Sequence s2 = fasta::make_sequence("ATSSA", 1);
  std::vector<fasta::SequenceList> input_multi = {{prof1, prof1},
                                                  {s1, s1}};
  fasta::SequenceList input_pairwise = {prof2, s2};
  std::vector<std::string> expected = {"A---A-",
                                       "A---A-",
                                       "AB--AD",
                                       "AB--AD",
                                       "ATSSA-",
                                       "ATSSA-"};

  std::vector<fasta::SequenceList> result = msa::add_alignment(input_multi,
                                                               input_pairwise);
  std::vector<std::string> result_str;
  for (auto& seqset : result) {
    for (auto& seq : seqset) {
      result_str.push_back(fasta::make_string(seq));
    }
  }
  BOOST_CHECK_EQUAL(result.size(), 3);
  BOOST_CHECK_EQUAL(result[0].size(), 2);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_str.begin(), result_str.end(),
                                expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_merge_alignments) {
  fasta::Sequence prof1 = fasta::make_sequence("A-A-", 1);
  fasta::Sequence s1 = fasta::make_sequence("ABAD", 1);
  fasta::Sequence prof2 = fasta::make_sequence("A---A", 1);
  fasta::Sequence s2 = fasta::make_sequence("ATSSA", 1);
  fasta::Sequence prof3 = fasta::make_sequence("---AA", 1);
  fasta::Sequence s3 = fasta::make_sequence("TSBAA", 1);
  std::vector<fasta::SequenceList> input_alignments = {{prof1, prof2, prof3},
                                                       {s1, s2, s3}};
  std::vector<fasta::SequenceList> result = msa::merge_alignments(
      input_alignments);
  std::vector<std::string> result_str;
  for (auto& seqset : result) {
    for (auto& seq : seqset) {
      result_str.push_back(fasta::make_string(seq));
    }
  }
  std::vector<std::string> expected = {"---AB--AD",
                                       "---ATSSA-",
                                       "TSBA---A-",
                                       "---AB--AD",
                                       "---ATSSA-",
                                       "TSBA---A-"};

  BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
                                result_str.begin(), result_str.end());
  fasta::Sequence prof4 = fasta::make_sequence("A-A-A-", 1);
  fasta::Sequence s4 = fasta::make_sequence("ABAD-C", 1);
  fasta::Sequence prof5 = fasta::make_sequence("A---AA-", 1);
  fasta::Sequence s5 = fasta::make_sequence("ATSSA-C", 1);
  fasta::Sequence prof6 = fasta::make_sequence("---AAA", 1);
  fasta::Sequence s6 = fasta::make_sequence("TSBAA-", 1);
  input_alignments = {{prof4, prof5, prof6}, {s4, s5, s6}};

  result = msa::merge_alignments(input_alignments);
  result_str.clear();
  for (auto& seqset : result) {
    for (auto& seq : seqset) {
      result_str.push_back(fasta::make_string(seq));
    }
  }
  expected = {"---AB--ADC",
              "---ATSSA-C",
              "TSBA---A--",
              "---AB--ADC",
              "---ATSSA-C",
              "TSBA---A--"};
  BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
                                result_str.begin(), result_str.end());
}

BOOST_AUTO_TEST_CASE(test_run_msa_with_feature_pattern) {
  fasta::Sequence s1 = fasta::make_sequence("WFQIANWFQWFQLAN", 1);
  fasta::Sequence s2 = fasta::make_sequence("WFQLANWFQWF", 1);
  fasta::SequenceList s = {s1, s2};
  f_config::FeatureSettingsMap f_set;
  bool gapped = false;
  fasta::FastaData fasta_data;
  fasta_data.sequences = s;
  seq_data::SequenceData sequence_data = seq_data::process_fasta_data(
      fasta_data, f_set, gapped);
  int domain_modifier = 4;
  int motif_modifier = 3;
  int ptm_modifier = 10;
  double gap_open_pen = -5;
  double gap_ext_pen = -1;
  double end_pen = -1;
  bool one_round = false;
  int codon_length = 7; 
  std::string sbst_mat = "BLOSUM";
  bool optimize = false;
  auto alignment = msa::run_msa(sequence_data, f_set, gap_open_pen, gap_ext_pen,
                                end_pen, domain_modifier, motif_modifier, 
                                ptm_modifier, codon_length, one_round, sbst_mat,
                                gapped, optimize);
  std::vector<std::string> expected = {"WFQIANWFQWFQLAN",
                                       "WFQLANWFQWF----",
                                       "WFQIANWFQWFQLAN",
                                       "WFQLANWFQWF----"};
  std::vector<std::string> result;
  for (auto& item : alignment) {
    for (auto& seq : item) {
      result.push_back(fasta::make_string(seq));
    }
  }
  BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
                                result.begin(), result.end());
  f_set = f_config::ConfParser::parse_conf_file("tests/test_conffile_pattern.cfg");
  sequence_data = seq_data::process_fasta_data(fasta_data, f_set, gapped);
  alignment = msa::run_msa(sequence_data, f_set, gap_open_pen, gap_ext_pen,
                                end_pen, domain_modifier, motif_modifier, 
                                ptm_modifier, codon_length, one_round, sbst_mat,
                                gapped, optimize);
  result.clear();
  for (auto& item : alignment) {
    for (auto& seq : item) {
      result.push_back(fasta::make_string(seq));
    }
  }
  expected = {"WFQIANWFQWFQLAN",
              "---------WFQLAN",
              "WFQIANWFQWFQLAN",
              "---------WFQLAn"};
  BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
                                result.begin(), result.end());
}

BOOST_AUTO_TEST_CASE(test_run_msa_sial_human) {
  f_config::FeatureSettingsMap f_set;
  fasta::SequenceList sequences;
  sequences = {fasta::make_sequence("GDNGEEGEEE", 1),
                fasta::make_sequence("GDNGEEGDQE", 1),
                fasta::make_sequence("GDNGEEDGEEE", 1),
                fasta::make_sequence("GDNGEEAEEA", 1),
                fasta::make_sequence("GDNGEEAEAEEA", 1)};
  int domain_modifier = 0;
  int motif_modifier = 0;
  int ptm_modifier = 0;
  double gap_open_pen = -12;
  double gap_ext_pen = -1;
  double end_pen = -12;
  int codon_length = 1;
  bool one_round = false;
  seq_data::SequenceData sequence_data;
  FeatureNamesList feature_list;
  std::map<std::string, double> probabilities;
  std::string sbst_mat = "DISORDER";
  bool first_gapped = true;
  sequence_data.feature_list = feature_list;

  std::vector<fasta::SequenceList> alignment;
  sequence_data.sequences = sequences;
  bool optimize = false;
  alignment = msa::run_msa(sequence_data, f_set, gap_open_pen, gap_ext_pen,
                           end_pen, domain_modifier, motif_modifier, 
                           ptm_modifier, codon_length, one_round, sbst_mat,
                           first_gapped, optimize);


  std::vector<std::string> result;
  for (auto& seqlist : alignment) {
    for (auto& seq : seqlist) {
      result.push_back(fasta::make_string(seq));
    }
  }
  std::vector<std::string> expected = {"GDNGEE--GEEE",
                                       "GDNGEE--GDQE",
                                       "GDNGEE-DGEEE",
                                       "GDNGEE--AEEA", 
                                       "GDNGEEAEAEEA", 
                                       "GDNGEE--GEEE", 
                                       "GDNGEE--GDQE", 
                                       "GDNGEE-DGEEE", 
                                       "GDNGEE--AEEA", 
                                       "GDNGEEAEAEEA"};

  //  std::cout << "SIAL_HUMAN test:" << std::endl;
  //  for (auto& seq : result) {
  //   std::cout << seq << std::endl;
  //  }
  BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                expected.begin(), expected.end());
}


BOOST_AUTO_TEST_SUITE_END()