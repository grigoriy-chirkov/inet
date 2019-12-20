package inet.showcases.wireless.spectrum;

import inet.environment.common.PhysicalEnvironment;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.node.wireless.Probe;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedVisualizer;


network SpectrumShowcaseBaseNetwork
{
    @display("bgb=661,400");
    submodules:
        host1: AdhocHost {
            @display("p=231,201");
        }
        host2: AdhocHost {
            @display("p=459,201");
        }
        host3: AdhocHost {
            @display("p=346,94");
        }
        host4: AdhocHost {
            @display("p=346,306");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=75,55");
        }
        visualizer: IntegratedVisualizer {
            @display("p=75,125");
        }
        probe: Probe {
            @display("p=100,100");
        }
}

network SpectrumShowcaseOneScalarRadioMediumModule extends SpectrumShowcaseBaseNetwork
{
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=74,191");
        }
}

network SpectrumShowcaseTwoScalarRadioMediumModules extends SpectrumShowcaseBaseNetwork
{
    submodules:
        radioMedium1: Ieee80211ScalarRadioMedium {
            @display("p=74,191");
        }
        radioMedium2: Ieee80211ScalarRadioMedium {
            @display("p=74,255");
        }
}

network SpectrumShowcasePartiallyOverlappingFrequencyBands extends SpectrumShowcaseBaseNetwork
{
    submodules:
        radioMedium: Ieee80211DimensionalRadioMedium {
            @display("p=74,184");
        }
        physicalEnvironment: PhysicalEnvironment {
            @display("p=65,257");
        }
}

network SpectrumShowcasePowerDensityMapNetwork
{
    @display("bgb=661,400");
    submodules:
        host1: AdhocHost {
            @display("p=150,125");
        }
        host2: AdhocHost {
            @display("p=320,201");
        }
        host3: AdhocHost {
            @display("p=280,120");
        }
        host4: AdhocHost {
            @display("p=240,200");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=75,55");
        }
        visualizer: IntegratedVisualizer {
            @display("p=75,125");
        }
        radioMedium: Ieee80211DimensionalRadioMedium {
            @display("p=74,184");
        }
        physicalEnvironment: PhysicalEnvironment {
            @display("p=65,257");
        }
        probe: Probe {
            @display("p=100,100");
        }
}
