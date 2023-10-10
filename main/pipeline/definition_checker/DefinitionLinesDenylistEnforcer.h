#ifndef SORBET_PIPELINE_DEFINITION_CHECKER
#define SORBET_PIPELINE_DEFINITION_CHECKER

#include "ast/Trees.h"
#include "core/Context.h"
#include "core/FileRef.h"

namespace sorbet::pipeline::definition_checker {

void checkNoDefinitionsInsideProhibitedLines(const core::GlobalState &gs, const UnorderedSet<core::FileRef> &files);

} // namespace sorbet::pipeline::definition_checker
#endif
