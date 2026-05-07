/**
 *
 * @file cli_configuration_parser_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests the CLI configuration parser.
 *
 * This file contains unit tests for configuration JSON parsing.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <rapidjson/document.h>

#include "SkeletonizationCLI/configuration/parser.hpp"
#include "SkeletonizationCLI/exceptions/configuration_exception.hpp"

using configuration::parse_image_entry;
using configuration::parse_variant;

namespace
{
    rapidjson::Value make_variant(rapidjson::Document& doc,
        const char* type,
        const char* algorithm)
    {
        rapidjson::Value variant(rapidjson::kObjectType);
        variant.AddMember("type", rapidjson::Value(type, doc.GetAllocator()), doc.GetAllocator());
        variant.AddMember("algorithm", rapidjson::Value(algorithm, doc.GetAllocator()), doc.GetAllocator());
        return variant;
    }
}

TEST(CliConfigurationParser, ParseVariantReadsFields)
{
    rapidjson::Document doc;
    doc.SetObject();

    auto variant = make_variant(doc, "cpu", "zhang_suen");
    const auto parsed = parse_variant(variant);

    EXPECT_EQ(parsed.type, "cpu");
    EXPECT_EQ(parsed.algorithm, "zhang_suen");
}

TEST(CliConfigurationParser, ParseVariantThrowsOnMissingFields)
{
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value variant(rapidjson::kObjectType);
    variant.AddMember("type", rapidjson::Value("cpu", doc.GetAllocator()), doc.GetAllocator());

    EXPECT_THROW(parse_variant(variant), cli::exceptions::configuration_validation_exception);
}

TEST(CliConfigurationParser, ParseImageEntryFailsOnEmptySkeletonizers)
{
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value entry(rapidjson::kObjectType);
    entry.AddMember("name", rapidjson::Value("Leaf", doc.GetAllocator()), doc.GetAllocator());
    entry.AddMember("path", rapidjson::Value("leaf.png", doc.GetAllocator()), doc.GetAllocator());
    rapidjson::Value skeletonizers(rapidjson::kArrayType);
    entry.AddMember("skeletonizers", skeletonizers, doc.GetAllocator());

    EXPECT_THROW(parse_image_entry(entry), cli::exceptions::configuration_validation_exception);
}

TEST(CliConfigurationParser, ParseImageEntryBuildsMetadata)
{
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value entry(rapidjson::kObjectType);
    entry.AddMember("name", rapidjson::Value("Leaf", doc.GetAllocator()), doc.GetAllocator());
    entry.AddMember("path", rapidjson::Value("leaf.png", doc.GetAllocator()), doc.GetAllocator());

    rapidjson::Value skeletonizers(rapidjson::kArrayType);
    skeletonizers.PushBack(make_variant(doc, "cpu", "zhang_suen"), doc.GetAllocator());
    entry.AddMember("skeletonizers", skeletonizers, doc.GetAllocator());

    const auto metadata = parse_image_entry(entry);
    EXPECT_EQ(metadata.name, "Leaf");
    EXPECT_EQ(metadata.path, "leaf.png");
    EXPECT_EQ(metadata.variants.size(), 1u);
    EXPECT_FALSE(metadata.skeletonizers.empty());
}

TEST(CliConfigurationParser, ParseImageEntryRejectsUnknownType)
{
    rapidjson::Document doc;
    doc.SetObject();

    rapidjson::Value entry(rapidjson::kObjectType);
    entry.AddMember("name", rapidjson::Value("Leaf", doc.GetAllocator()), doc.GetAllocator());
    entry.AddMember("path", rapidjson::Value("leaf.png", doc.GetAllocator()), doc.GetAllocator());

    rapidjson::Value skeletonizers(rapidjson::kArrayType);
    skeletonizers.PushBack(make_variant(doc, "unknown", "zhang_suen"), doc.GetAllocator());
    entry.AddMember("skeletonizers", skeletonizers, doc.GetAllocator());

    EXPECT_THROW(parse_image_entry(entry), cli::exceptions::configuration_validation_exception);
}
