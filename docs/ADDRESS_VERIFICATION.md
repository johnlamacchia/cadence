# Address verification: GD 2.2091

`tools/verify_addresses.py --sample 400 --seed 1` against `GeometryDash.exe` (6841 addresses in `addresses_2.2091.json`).

Every address in the file lands in an executable section: 6841/6841 (100.0%).

## Random sample of 400 addresses

| Check | Result |
|---|---|
| lands in an executable section | 400/400 (100.0%) |
| preceded by padding or the end of another function | 397/400 (99.2%) |
| decodes as code at the start | 400/400 (100.0%) |
| is referenced (call / jmp / lea / stored pointer) | 400/400 (100.0%) |

## Does the code behave like its name? (every address that can be tested)

| Check | Result |
|---|---|
| `X::create(...)` calls a function of class `X` or a related class (its init, or a base constructor), when it calls any named function | 149/150 (99.3%) |
| a virtual function's vtable neighbours belong to the same / a related class (when a neighbour is named) | 1894/1895 (99.9%) |

Not every miss is a wrong address: `create` may allocate and call an initialiser that the bindings mark `win inline`, and a vtable can interleave a base class's functions with no address. The sample checks above are the ones that cannot be explained that way.

Class sizes are a third, independent check: `tools/layout_check.py check` confirms that `sizeof(X)` read from each `X::create` in the exe equals the size computed from the bindings, which also requires that `create` was found at the right address.

## Sample addresses that failed a check

- `ThreadOptionsPopup::onModLock(cocos2d::CCObject* sender)`: not preceded by padding / the end of another function
- `EnhancedGameObject::init(char const* frame)`: not preceded by padding / the end of another function
- `ItemInfoPopup::onCredit(cocos2d::CCObject* sender)`: not preceded by padding / the end of another function
