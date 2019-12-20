package inet.showcases.wireless.spectrum2;

import inet.environment.common.PhysicalEnvironment;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.showcases.wireless.crosstalk.CrosstalkShowcaseBaseNetwork;
import inet.visualizer.integrated.IntegratedVisualizer;


network Spectrum2ShowcaseBaseNetwork
{
    @display("bgb=640,360");
    submodules:
        host1: AdhocHost {
            @display("p=230,180");
        }
        host2: AdhocHost {
            @display("p=370,180");
        }
        host3: AdhocHost {
            @display("p=290,130");
        }
        host4: AdhocHost {
            @display("p=440,130");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,50;is=s");
        }
        visualizer: IntegratedVisualizer {
            @display("p=50,100;is=s");
        }
        physicalEnvironment: PhysicalEnvironment {
            @display("p=50,150;is=s");
        }
}

network Spectrum2ShowcaseOneScalarRadioMediumModule extends Spectrum2ShowcaseBaseNetwork
{
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=74,191");
        }
}

network Spectrum2ShowcaseTwoScalarRadioMediumModules extends Spectrum2ShowcaseBaseNetwork
{
    submodules:
        radioMedium1: Ieee80211ScalarRadioMedium {
            @display("p=74,191");
        }
        radioMedium2: Ieee80211ScalarRadioMedium {
            @display("p=74,255");
        }
}

network Spectrum2ShowcasePartiallyOverlappingFrequencyBands extends Spectrum2ShowcaseBaseNetwork
{
    submodules:
        radioMedium: Ieee80211DimensionalRadioMedium {
            @display("p=50,200;is=s");
        }
}
