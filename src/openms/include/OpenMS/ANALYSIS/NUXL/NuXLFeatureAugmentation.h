// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2021.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Timo Sachsenberg $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/METADATA/PeptideHit.h>
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/ANALYSIS/NUXL/NuXLFeatureAugmentation.h>

#include <vector>

namespace OpenMS
{
  class OPENMS_DLLAPI NuXLFeatureAugmentation
  {
    public:
      static void augment(std::vector<PeptideIdentification>& pep_ids, const std::vector<std::string>& positive_weights)
      {
        // only for XLs? because they are fewer?
        if (pep_ids.empty()) return;

        if (pep_ids[0].getHits().empty()) return;

        // use first PSM as template
        auto p_template = pep_ids[0].getHits()[0];
        p_template.setScore(0);
        std::vector<String> keys;
        p_template.getKeys(keys);

        // clear scores of template
        for (const auto& k : keys)
        { 
          if (p_template.getMetaValue(k).valueType() == DataValue::INT_VALUE) p_template.setMetaValue(k, 0);
          if (p_template.getMetaValue(k).valueType() == DataValue::DOUBLE_VALUE) p_template.setMetaValue(k, 0.0);
        }

        size_t c = 0;
        // for each positive_weight feature, create one example with that feature set to large value
        for (const auto& s : positive_weights) 
        {
          auto p = p_template;
          p.setMetaValue(s, 1e7); // set feature value to a large value
          std::vector<PeptideHit> phs;
          phs.push_back(p);
          PeptideIdentification pid = pep_ids[0];
          pid.setRT(1e6 + c); // RT of augmented example
          pid.setHits(phs);
          pep_ids.push_back(pid);
          ++c;
        } 
      } 
  };
}

