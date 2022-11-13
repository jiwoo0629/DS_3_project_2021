#ifndef RESULT_H
#define RESULT_H

enum Result
{
    Success = 0,

    InvalidOptionNumber = 001,
    FailedtoUpdatePath = 005,

    CommandFileNotExist = 100,
    LoadFileNotExist = 101,
    

    InvalidVertexKey = 200,
    GraphNotExist = 202,
    InvalidAlgorithm = 203,
    NegativeCycleDetected = 204,

    NonDefinedCommand = 300
};

#endif

