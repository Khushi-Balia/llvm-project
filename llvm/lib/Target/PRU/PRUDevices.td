class FeatureSet<string name, string desc, list<SubtargetFeature> i>
    : SubtargetFeature<name, "m_FeatureSetDummy", "true", desc, i>;

// A family of microcontrollers, defining a set of supported features.
class Family<string name, list<SubtargetFeature> i>
    : FeatureSet<
          name, !strconcat("The device is a part of the ", name, " family"), i>;

def FamilyPRU0 : Family<"PRU0", []>;

class ELFArch<string name>
    : SubtargetFeature<"", "ELFArch", !strconcat("ELF::", name), "">;

def ELFArchPRU0 : ELFArch<"EF_ARM_EABI_UNKNOWN">;


class Device<string Name, Family Fam, ELFArch Arch,
             list<SubtargetFeature> ExtraFeatures = []>
    : Processor<Name, NoItineraries, !listconcat([Fam, Arch], ExtraFeatures)>;

def : Device<"PRU", FamilyPRU0, ELFArchPRU0>;