//
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_APSKDEMODULATOR_H
#define __INET_APSKDEMODULATOR_H

#include "inet/physicallayer/apskradio/bitlevel/ApskSymbol.h"
#include "inet/physicallayer/base/packetlevel/ApskModulationBase.h"
#include "inet/physicallayer/contract/bitlevel/IDemodulator.h"
#include "inet/physicallayer/contract/bitlevel/IDemodulator.h"
#include "inet/physicallayer/contract/bitlevel/ISignalBitModel.h"
#include "inet/physicallayer/contract/bitlevel/ISignalSymbolModel.h"

namespace inet {

namespace physicallayer {

class INET_API ApskDemodulator : public IDemodulator, public cSimpleModule
{
  protected:
    const ApskModulationBase *modulation;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;

  public:
    ApskDemodulator();

    virtual std::ostream& printToStream(std::ostream& stream, int level) const override;
    virtual const ApskModulationBase *getModulation() const { return modulation; }
    virtual const IReceptionBitModel *demodulate(const IReceptionSymbolModel *symbolModel) const override;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_APSKDEMODULATOR_H

