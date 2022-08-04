// This file is licensed under the Elastic License 2.0. Copyright 2021-present, StarRocks Limited.

package com.starrocks.rpc;

import com.baidu.brpc.client.RpcCallback;
import com.starrocks.lake.proto.AbortTxnRequest;
import com.starrocks.lake.proto.AbortTxnResponse;
import com.starrocks.lake.proto.CompactRequest;
import com.starrocks.lake.proto.CompactResponse;
import com.starrocks.lake.proto.DeleteDataRequest;
import com.starrocks.lake.proto.DeleteDataResponse;
import com.starrocks.lake.proto.DeleteTabletRequest;
import com.starrocks.lake.proto.DeleteTabletResponse;
import com.starrocks.lake.proto.PublishVersionRequest;
import com.starrocks.lake.proto.PublishVersionResponse;

import java.util.concurrent.Future;

public interface LakeServiceAsync extends LakeService  {
    Future<PublishVersionResponse> publishVersion(
            PublishVersionRequest request, RpcCallback<PublishVersionResponse> callback);

    Future<AbortTxnResponse> abortTxn(AbortTxnRequest request, RpcCallback<AbortTxnResponse> callback);

    Future<DeleteTabletResponse> deleteTablet(DeleteTabletRequest request, RpcCallback<DeleteTabletResponse> callback);

    Future<CompactResponse> compact(CompactRequest request, RpcCallback<CompactResponse> callback);

    Future<DeleteDataResponse> deleteData(DeleteDataRequest request, RpcCallback<DeleteDataResponse> callback);
}