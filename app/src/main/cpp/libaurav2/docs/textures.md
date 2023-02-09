# Textures

## Creation

Textures are packed using the [SpriteSheet Packer](https://github.com/amakaseev/sprite-sheet-packer) tool. The `.ssp` files that are used by this tool can found in the `assets` folder contains the base files for each texture, scaled to fit the UHD quality.

Upon building, the output can be found in the `assets/built` folder, with each quality given its own subfolder. These textures must be renamed to have a postfix of the texture quality. Some examples of renamed files are shown in the table below:


| Path                                  | Renamed Filename |
| ------------------------------------- | - |
| `assets/uhd/GJ_badgeSheet.png`        | `GJ_badgeSheet-uhd.png` |
| `assets/hd/GJ_badgeSheet.png`         | `GJ_badgeSheet-hd.png` |
| `assets/sd/GJ_modificationsSheet.png` | `GJ_modificationsSheet.png` |

The resulting file will be placed in the `patch/files/assets` folder, to be copied the next time the mod is built.

## Loading

Each module can define loaded spritesheets in the `LoadingLayer_loadAssets` function. As of right now, `GJ_modificationsSheet` loads before any other texture in the game. This means that if a texture is loaded in that sheet with the same name as a texture loaded by Geometry Dash, the mod's texture will override the main one. This is utilized to provide texture replacements. `GJ_badgeSheet` is loaded near-last, which means that this sheet will not replace other textures.
