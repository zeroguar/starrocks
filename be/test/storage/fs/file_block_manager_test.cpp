// This file is made available under Elastic License 2.0.
// This file is based on code available under the Apache license here:
//   https://github.com/apache/incubator-doris/blob/master/be/test/olap/fs/file_block_manager_test.cpp

// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "storage/fs/file_block_manager.h"

#include <gtest/gtest.h>

#include <string>

#include "fs/fs.h"
#include "testutil/assert.h"
#include "util/file_utils.h"
#include "util/slice.h"

using std::string;

namespace starrocks {

class FileBlockManagerTest : public testing::Test {
protected:
    const std::string kBlockManagerDir = "./ut_dir/file_block_manager";

    void SetUp() override {
        if (FileUtils::check_exist(kBlockManagerDir)) {
            ASSERT_TRUE(FileUtils::remove_all(kBlockManagerDir).ok());
        }
        ASSERT_TRUE(FileUtils::create_dir(kBlockManagerDir).ok());
    }

    void TearDown() override {
        if (FileUtils::check_exist(kBlockManagerDir)) {
            ASSERT_TRUE(FileUtils::remove_all(kBlockManagerDir).ok());
        }
    }
};

TEST_F(FileBlockManagerTest, NormalTest) {
    fs::BlockManagerOptions bm_opts;
    bm_opts.read_only = false;
    auto env = FileSystem::CreateSharedFromString("posix://").value();
    std::unique_ptr<fs::FileBlockManager> fbm(new fs::FileBlockManager(env, std::move(bm_opts)));

    std::unique_ptr<fs::WritableBlock> wblock;
    std::string fname = kBlockManagerDir + "/test_file";
    fs::CreateBlockOptions wblock_opts({fname});
    Status st = fbm->create_block(wblock_opts, &wblock);
    ASSERT_TRUE(st.ok()) << st.get_error_msg();

    std::string data = "abcdefghijklmnopqrstuvwxyz";
    wblock->append(data);
    wblock->close();

    ASSIGN_OR_ABORT(auto rf, fbm->new_random_access_file(fname));
    ASSIGN_OR_ABORT(auto file_size, rf->get_size());
    ASSERT_EQ(data.size(), file_size);
    std::string read_buff(data.size(), 'a');
    ASSERT_OK(rf->read_at_fully(0, read_buff.data(), read_buff.size()));
    ASSERT_EQ(data, read_buff);
}

} // namespace starrocks
