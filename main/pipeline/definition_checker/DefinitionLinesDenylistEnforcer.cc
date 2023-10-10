#include "main/pipeline/definition_checker/DefinitionLinesDenylistEnforcer.h"
#include "ast/treemap/treemap.h"

using namespace std;

namespace sorbet::pipeline::definition_checker {

namespace {

uint32_t outOfBoundsRange(const core::GlobalState &gs, core::FileRef file) {
    auto src = file.data(gs).source();
    auto n = src.size();
    ENFORCE(n % 2 == 1,
            "`{}` should have an odd number of characters in it, "
            "because we added one newline for every preexisting character, plus one extra.",
            file.data(gs).path());
    ENFORCE(src[0] == '\n' && src[n / 2] == '\n');

    return n / 2 + 1;
}

void checkSym(const core::GlobalState &gs, const UnorderedSet<core::FileRef> &files, core::SymbolRef sym) {
    for (const auto &loc : sym.locs(gs)) {
        auto file = loc.file();
        if (!files.contains(file)) {
            return;
        }

        auto midpoint = outOfBoundsRange(gs, file);
        ENFORCE(midpoint <= loc.beginPos(), "sym={} path={} midpoint={} beginPos={}", sym.show(gs),
                file.data(gs).path(), midpoint, loc.beginPos());
        ENFORCE(midpoint <= loc.endPos(), "sym={} path={} midpoint={} endPos={}", sym.show(gs), file.data(gs).path(),
                midpoint, loc.endPos());
    }
}

} // namespace

void checkNoDefinitionsInsideProhibitedLines(const core::GlobalState &gs, const UnorderedSet<core::FileRef> &files) {
    // TODO(jez) All symbol kinds
    for (int i = 1; i < gs.classAndModulesUsed(); i++) {
        auto sym = core::ClassOrModuleRef(gs, i);
        checkSym(gs, files, sym);
    }

    for (int i = 1; i < gs.methodsUsed(); i++) {
        auto sym = core::MethodRef(gs, i);
        checkSym(gs, files, sym);
    }

    for (int i = 1; i < gs.fieldsUsed(); i++) {
        auto sym = core::FieldRef(gs, i);
        checkSym(gs, files, sym);
    }

    for (int i = 1; i < gs.typeArgumentsUsed(); i++) {
        auto sym = core::TypeArgumentRef(gs, i);
        checkSym(gs, files, sym);
    }

    for (int i = 1; i < gs.typeMembersUsed(); i++) {
        auto sym = core::TypeMemberRef(gs, i);
        checkSym(gs, files, sym);
    }
}

} // namespace sorbet::pipeline::definition_checker
