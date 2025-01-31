/*
 * Copyright (c) 2020 YuQing <384681@qq.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _SERVER_GLOBAL_H
#define _SERVER_GLOBAL_H

#include "fastcommon/common_define.h"
#include "sf/sf_global.h"
#include "../common/vote_global.h"
#include "server_types.h"

typedef struct server_global_vars {
    struct {
        FCFSVoteClusterServerInfo *master;
        FCFSVoteClusterServerInfo *myself;
        FCFSVoteClusterServerInfo *next_master;
        SFClusterConfig config;
        FCFSVoteClusterServerArray server_array;

        struct {
            SFElectionQuorum quorum;
            bool vote_node_enabled;
            int master_lost_timeout;
            int max_wait_time;
        } master_election;

        SFContext sf_context;  //for cluster communication
    } cluster;

    SFSlowLogContext slow_log;
} VoteServerGlobalVars;

#define MASTER_ELECTION_QUORUM g_server_global_vars.cluster. \
    master_election.quorum
#define ELECTION_MASTER_LOST_TIMEOUT g_server_global_vars.cluster. \
    master_election.master_lost_timeout
#define ELECTION_MAX_WAIT_TIME   g_server_global_vars.cluster. \
    master_election.max_wait_time

#define CLUSTER_CONFIG          g_server_global_vars.cluster.config
#define CLUSTER_SERVER_CONFIG   CLUSTER_CONFIG.server_cfg

#define CLUSTER_NEXT_MASTER     g_server_global_vars.cluster.next_master
#define CLUSTER_MYSELF_PTR      g_server_global_vars.cluster.myself
#define CLUSTER_MASTER_PTR      g_server_global_vars.cluster.master
#define CLUSTER_MASTER_ATOM_PTR ((FCFSVoteClusterServerInfo *)  \
        __sync_add_and_fetch(&CLUSTER_MASTER_PTR, 0))
#define MYSELF_IS_MASTER        (CLUSTER_MASTER_ATOM_PTR == CLUSTER_MYSELF_PTR)

#define CLUSTER_SERVER_ARRAY    g_server_global_vars.cluster.server_array
#define CLUSTER_MY_SERVER_ID    CLUSTER_MYSELF_PTR->server->id

#define CLUSTER_SF_CTX          g_server_global_vars.cluster.sf_context

#define SLOW_LOG                g_server_global_vars.slow_log
#define SLOW_LOG_CFG            SLOW_LOG.cfg
#define SLOW_LOG_CTX            SLOW_LOG.ctx

#define CLUSTER_GROUP_INDEX     g_server_global_vars.cluster.config.cluster_group_index
#define SERVICE_GROUP_INDEX     g_server_global_vars.cluster.config.service_group_index

#define CLUSTER_GROUP_ADDRESS_ARRAY(server) \
    (server)->group_addrs[CLUSTER_GROUP_INDEX].address_array
#define SERVICE_GROUP_ADDRESS_ARRAY(server) \
    (server)->group_addrs[SERVICE_GROUP_INDEX].address_array

#define CLUSTER_GROUP_ADDRESS_FIRST_PTR(server) \
    (*(server)->group_addrs[CLUSTER_GROUP_INDEX].address_array.addrs)
#define SERVICE_GROUP_ADDRESS_FIRST_PTR(server) \
    (*(server)->group_addrs[SERVICE_GROUP_INDEX].address_array.addrs)

#define CLUSTER_GROUP_ADDRESS_FIRST_IP(server) \
    CLUSTER_GROUP_ADDRESS_FIRST_PTR(server)->conn.ip_addr
#define CLUSTER_GROUP_ADDRESS_FIRST_PORT(server) \
    CLUSTER_GROUP_ADDRESS_FIRST_PTR(server)->conn.port

#define SERVICE_GROUP_ADDRESS_FIRST_IP(server) \
    SERVICE_GROUP_ADDRESS_FIRST_PTR(server)->conn.ip_addr
#define SERVICE_GROUP_ADDRESS_FIRST_PORT(server) \
    SERVICE_GROUP_ADDRESS_FIRST_PTR(server)->conn.port

#define CLUSTER_CONFIG_SIGN_BUF g_server_global_vars.cluster.config.md5_digest

#ifdef __cplusplus
extern "C" {
#endif

    extern VoteServerGlobalVars g_server_global_vars;

#ifdef __cplusplus
}
#endif

#endif
