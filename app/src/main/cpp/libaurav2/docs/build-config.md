# Build Configuration

The following configuration variables modify the build output:

| CMake Variable         | Macro                    | Description |
| ---------------------- | ------------------------ | - |
| GDMOD_USE_IMGUI        | GDMOD_ENABLE_LOGGING     | Enables logging overlay through ImGui. ImGui will not be linked if this is disabled, and the overlay will be hidden. |
| GDMOD_USE_SPDLOG       | GDMOD_ENABLE_SPDLOG      | Enables logging through spdlog. spdlog will not be linked if this is disabled, and will be replaced by a stubbed version that gets optimized out on compile time. Enabling this heavily increases library size. |
| GDMOD_USE_CRASH_DUMPS  | GDMOD_ENABLE_CRASH_DUMPS | Enables Breakpad on run. Breakpad is linked regardless of this variable's status. If disabled, stacktraces can be found through logcat. |
| GDMOD_CRASH_UPLOAD_URL | GDMOD_CRASH_UPLOAD_URL   | URL where saved Breakpad dumps can be uploaded to. See [crash-reporter.md](crash-reporter.md) for more details. |
| GDMOD_SOURCE_URL       | GDMOD_SOURCE_URL         | URL to the mod's repository. |