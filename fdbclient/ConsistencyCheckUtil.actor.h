/*
 * ConsistencyCheckUtil.actor.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_CONSISTENCYCHECKUTIL_ACTOR_G_H)
#define FDBCLIENT_CONSISTENCYCHECKUTIL_ACTOR_G_H
#include "fdbclient/ConsistencyCheckUtil.actor.g.h"
#elif !defined(FDBCLIENT_CONSISTENCYCHECKUTIL_ACTOR_H)
#define FDBCLIENT_CONSISTENCYCHECKUTIL_ACTOR_H

#include "fdbclient/ConsistencyCheck.h"
#include "fdbclient/DatabaseContext.h"
#include "fdbclient/SystemData.h"

// Key invariant
// Only persistConsistencyCheckerId() directly overwrites the consistencyCheckerId
// Other APIs check the consistencyCheckerId on disk before updating/reading from disk
// This invariant guarantees that there is at most one consistency checker --- the
// last checker of overwriting consistencyCheckerIdKey wins.
// Other checkers will receive consistency_check_task_outdated error and the checker should
// abort itself.

ACTOR Future<std::vector<KeyRange>> loadRangesToCheckFromAssignmentMetadata(Database cx,
                                                                            int clientId,
                                                                            int64_t consistencyCheckerId);

ACTOR Future<Void> persistConsistencyCheckProgress(Database cx, KeyRange range, int64_t consistencyCheckerId);

ACTOR Future<Void> initConsistencyCheckAssignmentMetadata(Database cx, int64_t consistencyCheckerId);

ACTOR Future<Void> initConsistencyCheckProgressMetadata(Database cx,
                                                        std::vector<KeyRange> rangesToCheck,
                                                        int64_t consistencyCheckerId);

ACTOR Future<Void> persistConsistencyCheckerId(Database cx, int64_t consistencyCheckerId);

ACTOR Future<Void> clearConsistencyCheckMetadata(Database cx, int64_t consistencyCheckerId);

ACTOR Future<std::vector<KeyRange>> loadRangesToCheckFromProgressMetadata(Database cx, int64_t consistencyCheckerId);

ACTOR Future<Void> persistConsistencyCheckAssignment(Database cx,
                                                     int clientId,
                                                     std::vector<KeyRange> assignedRanges,
                                                     int64_t consistencyCheckerId);

std::vector<KeyRange> loadRangesToCheckFromKnob();

#include "flow/unactorcompiler.h"
#endif
