/**
 *  @file   LArContent/include/LArPersistency/EventWritingAlgorithm.h
 * 
 *  @brief  Header file for the event writing algorithm class.
 * 
 *  $Log: $
 */
#ifndef LAR_EVENT_WRITING_ALGORITHM_H
#define LAR_EVENT_WRITING_ALGORITHM_H 1

#include "Pandora/Algorithm.h"

#include "Persistency/PandoraIO.h"

namespace pandora {class FileWriter;}

//------------------------------------------------------------------------------------------------------------------------------------------

namespace lar_content
{

/**
 *  @brief  EventWritingAlgorithm class
 */
class EventWritingAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

    /**
     *  @brief  Default constructor
     */
    EventWritingAlgorithm();

    /**
     *  @brief  Destructor
     */
    ~EventWritingAlgorithm();

private:
    pandora::StatusCode Initialize();
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    pandora::FileType       m_geometryFileType;             ///< The geometry file type
    pandora::FileType       m_eventFileType;                ///< The event file type

    bool                    m_shouldWriteGeometry;          ///< Whether to write geometry to a specified file
    bool                    m_writtenGeometry;              ///< Whether geometry has been written
    std::string             m_geometryFileName;             ///< Name of the output geometry file

    bool                    m_shouldWriteEvents;            ///< Whether to write events to a specified file
    std::string             m_eventFileName;                ///< Name of the output event file

    bool                    m_shouldWriteMCRelationships;   ///< Whether to write mc relationship information to the events file
    bool                    m_shouldWriteTrackRelationships;///< Whether to write track relationship information to the events file

    bool                    m_shouldOverwriteEventFile;     ///< Whether to overwrite existing event file with specified name, or append
    bool                    m_shouldOverwriteGeometryFile;  ///< Whether to overwrite existing geometry file with specified name, or append

    bool                    m_shouldFilterByNuanceCode;     ///< Whether to filter output by nuance code
    int                     m_filterNuanceCode;             ///< The filter nuance code (required if specify filter by nuance code)
    std::string             m_mcParticleListName;           ///< Name of input MC particle list (required if specify filter by nuance code)

    pandora::FileWriter    *m_pEventFileWriter;             ///< Address of the event file writer
    pandora::FileWriter    *m_pGeometryFileWriter;          ///< Address of the geometry file writer
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *EventWritingAlgorithm::Factory::CreateAlgorithm() const
{
    return new EventWritingAlgorithm();
}

} // namespace lar_content

#endif // #ifndef LAR_EVENT_WRITING_ALGORITHM_H