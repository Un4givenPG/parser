#include "zusi_parser/utils.hpp"

#include <boost/test/unit_test.hpp>

#include <string_view>

using namespace std::string_view_literals;
using namespace zusixml;

BOOST_AUTO_TEST_SUITE(ZusiPfadTest)

BOOST_AUTO_TEST_CASE(ZusiPfad_Konstruktoren) {
  const auto zusiPfad1 = ZusiPfad::vonZusiPfad("RollingStock\\Test");
  BOOST_TEST(zusiPfad1.alsZusiPfad() == "RollingStock\\Test");
  const auto zusiPfad2 = zusiPfad1;
  BOOST_TEST(zusiPfad2.alsZusiPfad() == "RollingStock\\Test");
  const auto zusiPfad3 = std::move(zusiPfad2);
  BOOST_TEST(zusiPfad3.alsZusiPfad() == "RollingStock\\Test");
}

BOOST_AUTO_TEST_CASE(ZusiPfad_vonZusiPfad_ohneUebergeordnet) {
  const auto zusiPfad1 = ZusiPfad::vonZusiPfad("RollingStock\\Test");
  BOOST_TEST(zusiPfad1.alsZusiPfad() == "RollingStock\\Test");

  const auto zusiPfad2 = ZusiPfad::vonZusiPfad("RollingStock\\Test\\");
  BOOST_TEST(zusiPfad2.alsZusiPfad() == "RollingStock\\Test\\");

  const auto zusiPfad3 = ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3");
  BOOST_TEST(zusiPfad3.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad4 = ZusiPfad::vonZusiPfad("\\RollingStock\\Test\\test.ls3");
  BOOST_TEST(zusiPfad4.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad5 = ZusiPfad::vonZusiPfad("");
  BOOST_TEST(zusiPfad5.alsZusiPfad() == "");

  const auto zusiPfad6 = ZusiPfad::vonZusiPfad("\\");
  BOOST_TEST(zusiPfad6.alsZusiPfad() == "");
}

BOOST_AUTO_TEST_CASE(ZusiPfad_vonZusiPfad_mitUebergeordnet) {
  const auto zusiPfad1 = ZusiPfad::vonZusiPfad("test2.ls3"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test"));
  BOOST_TEST(zusiPfad1.alsZusiPfad() == "RollingStock\\test2.ls3");

  const auto zusiPfad2 = ZusiPfad::vonZusiPfad("test2.ls3"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\"));
  BOOST_TEST(zusiPfad2.alsZusiPfad() == "RollingStock\\Test\\test2.ls3");

  const auto zusiPfad3 = ZusiPfad::vonZusiPfad("test2.ls3"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad3.alsZusiPfad() == "RollingStock\\Test\\test2.ls3");

  const auto zusiPfad4 = ZusiPfad::vonZusiPfad("test2.ls3"sv, ZusiPfad::vonZusiPfad(""));
  BOOST_TEST(zusiPfad4.alsZusiPfad() == "test2.ls3");

  const auto zusiPfad5 = ZusiPfad::vonZusiPfad("test2.ls3"sv, ZusiPfad::vonZusiPfad("\\"));
  BOOST_TEST(zusiPfad5.alsZusiPfad() == "test2.ls3");

  const auto zusiPfad6 = ZusiPfad::vonZusiPfad("Test2\\test2.ls3"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad6.alsZusiPfad() == "Test2\\test2.ls3");

  const auto zusiPfad7 = ZusiPfad::vonZusiPfad("\\Test2\\test2.ls3"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad7.alsZusiPfad() == "Test2\\test2.ls3");

  const auto zusiPfad8 = ZusiPfad::vonZusiPfad("Test2\\"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad8.alsZusiPfad() == "Test2\\");

  const auto zusiPfad9 = ZusiPfad::vonZusiPfad("\\"sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad9.alsZusiPfad() == "");

  const auto zusiPfad10 = ZusiPfad::vonZusiPfad(""sv, ZusiPfad::vonZusiPfad("RollingStock\\Test\\test.ls3"));
  BOOST_TEST(zusiPfad10.alsZusiPfad() == "");
}

#ifdef _WIN32
BOOST_AUTO_TEST_CASE(ZusiPfad_vonOsPfad) {
  BOOST_TEST_REQUIRE(getZusiDatenpfad() == "C:\\Users\\vmuser\\ZusiDaten\\");
  BOOST_TEST_REQUIRE(getZusiDatenpfadOffiziell() == "C:\\Users\\Public\\Documents\\_Zusidata\\");

  const auto zusiPfad1 = ZusiPfad::vonOsPfad("C:\\Users\\vmuser\\ZusiDaten\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad1.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad2 = ZusiPfad::vonOsPfad("C:\\Users\\Public\\Documents\\_Zusidata\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad2.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad3 = ZusiPfad::vonOsPfad("C:\\Users\\Public\\Documents\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad3.alsZusiPfad() == "..\\..\\Public\\Documents\\RollingStock\\Test\\test.ls3");  // relativ zu eigenem Datenverzeichnis, da der Pfad nicht im offiziellen Verzeichnis liegt!

  const auto zusiPfad4 = ZusiPfad::vonOsPfad("C:\\Users\\vmuser\\ZusiDaten\\RollingStock\\Test\\"sv);
  BOOST_TEST(zusiPfad4.alsZusiPfad() == "RollingStock\\Test\\");

  const auto zusiPfad5 = ZusiPfad::vonOsPfad("C:\\Users\\vmuser\\ZusiDaten\\"sv);
  BOOST_TEST(zusiPfad5.alsZusiPfad() == "");

  const auto zusiPfad6 = ZusiPfad::vonOsPfad("C:\\Users\\vmuser\\ZusiDaten"sv);
  BOOST_TEST(zusiPfad6.alsZusiPfad() == "");

  const auto zusiPfad7 = ZusiPfad::vonOsPfad("D:\\zusi\\..\\Zusi3\\Daten\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad7.alsZusiPfad() == "D:\\Zusi3\\Daten\\RollingStock\\Test\\test.ls3");

  const auto zusiPfad8 = ZusiPfad::vonOsPfad("c:\\uSers\\vMuser\\zUsidAten\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad8.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad9 = ZusiPfad::vonOsPfad("c:\\uSers\\pUblic\\dOcuments\\_zUsidata\\RollingStock\\Test\\test.ls3"sv);
  BOOST_TEST(zusiPfad9.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

}
#else
BOOST_AUTO_TEST_CASE(ZusiPfad_vonOsPfad) {
  BOOST_TEST_REQUIRE(getZusiDatenpfad() == "/mnt/zusi/Daten/");
  BOOST_TEST_REQUIRE(getZusiDatenpfadOffiziell() == "/mnt/zusi/Offiziell/Daten/");

  const auto zusiPfad1 = ZusiPfad::vonOsPfad("/mnt/zusi/Daten/RollingStock/Test/test.ls3"sv);
  BOOST_TEST(zusiPfad1.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad2 = ZusiPfad::vonOsPfad("/mnt/zusi/Offiziell/Daten/RollingStock/Test/test.ls3"sv);
  BOOST_TEST(zusiPfad2.alsZusiPfad() == "RollingStock\\Test\\test.ls3");

  const auto zusiPfad3 = ZusiPfad::vonOsPfad("/mnt/zusi/Offiziell/RollingStock/Test/test.ls3"sv);
  BOOST_TEST(zusiPfad3.alsZusiPfad() == "..\\Offiziell\\RollingStock\\Test\\test.ls3");  // relativ zu eigenem Datenverzeichnis, da der Pfad nicht im offiziellen Verzeichnis liegt!

  const auto zusiPfad4 = ZusiPfad::vonOsPfad("/mnt/zusi/Daten/RollingStock/Test/"sv);
  BOOST_TEST(zusiPfad4.alsZusiPfad() == "RollingStock\\Test\\");

  const auto zusiPfad5 = ZusiPfad::vonOsPfad("/mnt/zusi/Daten/"sv);
  BOOST_TEST(zusiPfad5.alsZusiPfad() == "");

  const auto zusiPfad6 = ZusiPfad::vonOsPfad("/mnt/zusi/Daten"sv);
  BOOST_TEST(zusiPfad6.alsZusiPfad() == "");

  const auto zusiPfad7 = ZusiPfad::vonOsPfad("../../../../../../../../mnt/zusi/Daten/RollingStock/Test/test.ls3"sv);
  BOOST_TEST(zusiPfad7.alsZusiPfad() == "RollingStock\\Test\\test.ls3");
}
#endif


BOOST_AUTO_TEST_CASE(ZusiPfad_vonOsPfad_AndereInstallation) {
  const std::string_view expected = "Routes\\Deutschland\\Test\\Test.st3";
#ifdef _WIN32
  const auto module = ZusiPfad::vonOsPfad("X:/routegraph2-test/Other Zusi/_ZusiData/Routes/Deutschland/Test/Test.st3");
  BOOST_TEST(module.alsZusiPfad() == expected);
  BOOST_TEST(ZusiPfad::vonOsPfad("X:\\routegraph2-test\\Other Zusi\\_zUsIdAtA\\Routes\\Deutschland\\Test\\Test.st3").alsZusiPfad() == expected);
  BOOST_TEST(ZusiPfad::vonOsPfad("X:/routegraph2-test/_ZusiDatabase/Routes/Deutschland/Test/Test.st3").alsZusiPfad() != expected);
#else
  const auto module = ZusiPfad::vonOsPfad("/tmp/routegraph2-test/Other Zusi/_ZusiData/Routes/Deutschland/Test/Test.st3");
  BOOST_TEST(module.alsZusiPfad() == expected);
  BOOST_TEST(ZusiPfad::vonOsPfad("/tmp/routegraph2-test/_ZusiDatabase/Routes/Deutschland/Test/Test.st3").alsZusiPfad() != expected);
#endif
  BOOST_TEST(ZusiPfad::vonZusiPfad("Test.st3", module).alsZusiPfad() == expected);
  BOOST_TEST(ZusiPfad::vonOsPfad(getZusiDatenpfadOffiziell() + "Routes/Deutschland/Test/Test.st3").alsZusiPfad() == expected);
  BOOST_TEST(ZusiPfad::vonOsPfad(getZusiDatenpfad() + "Routes/Deutschland/Test/Test.st3").alsZusiPfad() == expected);
}

BOOST_AUTO_TEST_CASE(ZusiPfad_vonOsPfad_AndereInstallation_Referenzen) {
#ifdef _WIN32
  const std::string root = "X:/routegraph2-path-test/First/_ZusiData/";
  const std::string otherRoot = "X:/routegraph2-path-test/Second/_ZusiData/";
#else
  const std::string root = "/tmp/routegraph2-path-test/First/_ZusiData/";
  const std::string otherRoot = "/tmp/routegraph2-path-test/Second/_ZusiData/";
#endif
  const auto normalisiere = [](const std::string& pfad) {
    return fs::path(pfad).make_preferred().string();
  };
  const std::string configuredRoot = getZusiDatenpfadOffiziell();
  const auto timetable = ZusiPfad::vonOsPfad(root + "Timetables/Test/Test.fpn");
  BOOST_TEST(normalisiere(timetable.alsOsPfad()) == normalisiere(root + "Timetables/Test/Test.fpn"));

  const auto module = ZusiPfad::vonZusiPfad("Routes\\routegraph2-path-test\\Test.st3", timetable);
  BOOST_TEST(module.alsZusiPfad() == "Routes\\routegraph2-path-test\\Test.st3");
  BOOST_TEST(normalisiere(module.alsOsPfad()) == normalisiere(root + "Routes/routegraph2-path-test/Test.st3"));
  BOOST_TEST(normalisiere(ZusiPfad::vonZusiPfad("\\Routes\\routegraph2-path-test\\Test.st3", timetable).alsOsPfad())
      == normalisiere(root + "Routes/routegraph2-path-test/Test.st3"));
  BOOST_TEST(normalisiere(ZusiPfad::vonZusiPfad("Sibling.st3", module).alsOsPfad())
      == normalisiere(root + "Routes/routegraph2-path-test/Sibling.st3"));

  const auto other = ZusiPfad::vonOsPfad(otherRoot + "Timetables/Test/Test.fpn");
  BOOST_TEST(normalisiere(ZusiPfad::vonZusiPfad("Routes\\routegraph2-path-test\\Test.st3", other).alsOsPfad())
      == normalisiere(otherRoot + "Routes/routegraph2-path-test/Test.st3"));
  BOOST_TEST(normalisiere(module.alsOsPfad()) == normalisiere(root + "Routes/routegraph2-path-test/Test.st3"));
  const auto copied = module;
  BOOST_TEST(normalisiere(copied.alsOsPfad()) == normalisiere(root + "Routes/routegraph2-path-test/Test.st3"));
  const auto bare = ZusiPfad::vonOsPfad(root + "Test.fpn");
  BOOST_TEST(normalisiere(ZusiPfad::vonZusiPfad("Child.st3", bare).alsOsPfad()) == normalisiere(root + "Child.st3"));
  BOOST_TEST(getZusiDatenpfadOffiziell() == configuredRoot);
}

BOOST_AUTO_TEST_SUITE_END()
