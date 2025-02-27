#pragma once

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

// The max engine workers that can be expected to run within a callback
// (e.g. the max that we will schedule). Must be a power of 2.
#define MAX_ENGINE_WORKERS 32

class EngineWorker;

class EngineWorkerScheduler : public QThread {
    Q_OBJECT
  public:
    EngineWorkerScheduler(QObject* pParent=NULL);
    virtual ~EngineWorkerScheduler();

    void addWorker(EngineWorker* pWorker);
    void runWorkers();
    void workerReady();

  protected:
    void run();

  private:
    // Indicates whether workerReady has been called since the last time
    // runWorkers was run. This should only be touched from the engine callback.
    std::atomic<bool> m_bWakeScheduler;

    QWaitCondition m_waitCondition;

    // mutex protects m_workers and m_bQuit
    QMutex m_mutex;
    std::vector<EngineWorker*> m_workers;
    bool m_bQuit;
};
