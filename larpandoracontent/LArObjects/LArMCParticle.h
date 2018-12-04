/**
 *  @file   larpandoracontent/LArObjects/LArMCParticle.h
 * 
 *  @brief  Header file for the lar mc particle class.
 * 
 *  $Log: $
 */
#ifndef LAR_MC_PARTICLE_H
#define LAR_MC_PARTICLE_H 1

#include "Objects/MCParticle.h"

#include "Pandora/ObjectCreation.h"
#include "Pandora/PandoraObjectFactories.h"

#include "Persistency/BinaryFileReader.h"
#include "Persistency/BinaryFileWriter.h"
#include "Persistency/XmlFileReader.h"
#include "Persistency/XmlFileWriter.h"

namespace lar_content
{

/**
 *  @brief  LAr mc particle parameters
 */
class LArMCParticleParameters : public object_creation::MCParticle::Parameters
{
public:
    pandora::InputInt   m_nuanceCode;               ///< The nuance code
    pandora::InputString m_Process;
    pandora::InputString m_endProcess;
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  LAr mc particle class
 */
class LArMCParticle : public object_creation::MCParticle::Object
{
public:
    /**
     *  @brief  Constructor
     * 
     *  @param  parameters the lar mc particle parameters
     */
    LArMCParticle(const LArMCParticleParameters &parameters);

    /**
     *  @brief  Get the nuance code
     * 
     *  @return the nuance code
     */
    int GetNuanceCode() const;
    /**
     *  @brief  Get the process
     *           
     *  @return process
     */
    std::string GetProcess() const;
    /**
     *  @brief  Get the end process
     *                      
     *  @return end process
     */
    std::string GetEndProcess() const;

private:
    int                 m_nuanceCode;               ///< The nuance code
    std::string         m_Process;
    std::string         m_endProcess;
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  LArMCParticleFactory responsible for object creation
 */
class LArMCParticleFactory : public pandora::ObjectFactory<object_creation::MCParticle::Parameters, object_creation::MCParticle::Object>
{
public:
    /**
     *  @brief  Create new parameters instance on the heap (memory-management to be controlled by user)
     * 
     *  @return the address of the new parameters instance
     */
    Parameters *NewParameters() const;

    /**
     *  @brief  Read any additional (derived class only) object parameters from file using the specified file reader
     *
     *  @param  parameters the parameters to pass in constructor
     *  @param  fileReader the file reader, used to extract any additional parameters from file
     */
    pandora::StatusCode Read(Parameters &parameters, pandora::FileReader &fileReader) const;

    /**
     *  @brief  Persist any additional (derived class only) object parameters using the specified file writer
     *
     *  @param  pObject the address of the object to persist
     *  @param  fileWriter the file writer
     */
    pandora::StatusCode Write(const Object *const pObject, pandora::FileWriter &fileWriter) const;

    /**
     *  @brief  Create an object with the given parameters
     *
     *  @param  parameters the parameters to pass in constructor
     *  @param  pObject to receive the address of the object created
     */
    pandora::StatusCode Create(const Parameters &parameters, const Object *&pObject) const;
};

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

inline LArMCParticle::LArMCParticle(const LArMCParticleParameters &parameters) :
    object_creation::MCParticle::Object(parameters),
    m_nuanceCode(parameters.m_nuanceCode.Get()),
    m_Process(parameters.m_Process.Get()),
    m_endProcess(parameters.m_endProcess.Get())
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArMCParticle::GetNuanceCode() const
{
    return m_nuanceCode;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string LArMCParticle::GetProcess() const
{
    return m_Process;
}
//-----------------------------------------------------------------------------------------------------------------------------------------

inline std::string LArMCParticle::GetEndProcess() const
{
    return m_endProcess;
}
//------------------------------------------------------------------------------------------------------------------------------------------

inline LArMCParticleFactory::Parameters *LArMCParticleFactory::NewParameters() const
{
    return (new LArMCParticleParameters);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode LArMCParticleFactory::Create(const Parameters &parameters, const Object *&pObject) const
{
    const LArMCParticleParameters &larMCParticleParameters(dynamic_cast<const LArMCParticleParameters&>(parameters));
    pObject = new LArMCParticle(larMCParticleParameters);

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode LArMCParticleFactory::Read(Parameters &parameters, pandora::FileReader &fileReader) const
{
    // ATTN: To receive this call-back must have already set file reader mc particle factory to this factory
    int nuanceCode(0);
    std::string Process;
    std::string endProcess;

    if (pandora::BINARY == fileReader.GetFileType())
    {
        pandora::BinaryFileReader &binaryFileReader(dynamic_cast<pandora::BinaryFileReader&>(fileReader));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileReader.ReadVariable(nuanceCode));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileReader.ReadVariable(Process));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileReader.ReadVariable(endProcess));
    }
    else if (pandora::XML == fileReader.GetFileType())
    {
        pandora::XmlFileReader &xmlFileReader(dynamic_cast<pandora::XmlFileReader&>(fileReader));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileReader.ReadVariable("NuanceCode", nuanceCode));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileReader.ReadVariable("Process", Process));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileReader.ReadVariable("endProcess", endProcess));
    }
    else
    {
        return pandora::STATUS_CODE_INVALID_PARAMETER;
    }

    LArMCParticleParameters &larMCParticleParameters(dynamic_cast<LArMCParticleParameters&>(parameters));
    larMCParticleParameters.m_nuanceCode = nuanceCode;
    larMCParticleParameters.m_Process = Process;
    larMCParticleParameters.m_endProcess = endProcess;

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode LArMCParticleFactory::Write(const Object *const pObject, pandora::FileWriter &fileWriter) const
{
    // ATTN: To receive this call-back must have already set file writer mc particle factory to this factory
    const LArMCParticle *const pLArMCParticle(dynamic_cast<const LArMCParticle*>(pObject));

    if (!pLArMCParticle)
        return pandora::STATUS_CODE_INVALID_PARAMETER;

    if (pandora::BINARY == fileWriter.GetFileType())
    {
        pandora::BinaryFileWriter &binaryFileWriter(dynamic_cast<pandora::BinaryFileWriter&>(fileWriter));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileWriter.WriteVariable(pLArMCParticle->GetNuanceCode()));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileWriter.WriteVariable(pLArMCParticle->GetProcess()));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, binaryFileWriter.WriteVariable(pLArMCParticle->GetEndProcess()));

    }
    else if (pandora::XML == fileWriter.GetFileType())
    {
        pandora::XmlFileWriter &xmlFileWriter(dynamic_cast<pandora::XmlFileWriter&>(fileWriter));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileWriter.WriteVariable("NuanceCode", pLArMCParticle->GetNuanceCode()));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileWriter.WriteVariable("Process", pLArMCParticle->GetProcess()));
        PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, xmlFileWriter.WriteVariable("endProcess", pLArMCParticle->GetEndProcess()));
    }
    else
    {
        return pandora::STATUS_CODE_INVALID_PARAMETER;
    }

    return pandora::STATUS_CODE_SUCCESS;
}

} // namespace lar_content

#endif // #ifndef LAR_MC_PARTICLE_H
